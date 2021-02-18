#include "network.hpp"
#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

const unsigned short PORT = 8080;
sf::IpAddress IP = sf::IpAddress::getLocalAddress();

Server::Server() {
  _listener.listen(PORT, IP);
  _running = false;
}

std::vector<commandes> Server::receive() {
  std::vector<commandes> result(2, none);
  std::lock_guard<std::mutex> lock(_mtx);
  for (auto &client : _clients) {
    result[client.id] = static_cast<commandes>(client.input[0]);
  }
  return result;
}

void Server::run() {
  _running = true;

  int total_nb = 1;
  while (_clients.size() < total_nb) {
    std::cout << "En attente de client" << std::endl;

    auto socket = std::make_shared<sf::TcpSocket>();
    auto status = _listener.accept(*socket);
    if (status != sf::Socket::Done) {
      continue;
    }

    sf::Packet id_packet;
    std::string id;
    if (socket->receive(id_packet) == sf::Socket::Done) {
      id_packet >> id;
    } else {
      continue;
    }
    std::cout << "Player " << id << " is connected (" << _clients.size() << "/"
              << total_nb << ") - IP : " << socket->getRemoteAddress()
              << std::endl;

    _clients.emplace_back(_clients.size(), id, socket);
  }

  std::cout << "Connection success" << std::endl;

  // Start a thread by client to listen
  std::vector<std::thread> threads;
  for (auto &client : _clients) {
    threads.emplace_back([this, &client]() {
      while (_running) {
        std::string input;
        { // Receive
          sf::Packet packet;
          auto status = client.socket->receive(packet);
          if (status == sf::Socket::Done) {
            packet >> input;
          } else {
            std::cout << "Exiting" << std::endl;
            return;
          }
          std::cout << "Received from client " << client.id << ": " << input
                    << std::endl;

          if (input == "exit") {
            std::cout << "Client " << client.id << " disconnected" << std::endl;
            return;
          }

          if (input == "ping") {
            // dismiss
            continue;
          }
        }

        sf::Packet board;
        {
          std::lock_guard<std::mutex> lock(_mtx);
          client.input = input;
          board << client.state;
        }

        { // Reply
          std::cout << "Sending update to client " << client.id << std::endl;
          auto status = client.socket->send(board);
          if (status != sf::Socket::Done) {
            std::cout << "Disconnected from client" << std::endl;
            return;
          }
        }
      }
    });
  }

  for (auto &thread : threads) {
    thread.join();
  }
}

Client::Client() {
  _socket.connect(IP, PORT);
  _running = false;
  _is_connected = false;
  std::cout << "You are connected as a Player" << std::endl;
}

void Client::run() {
  _running = true;
  std::string id;
  std::cout << "Enter online id: ";
  std::cin >> id;
  if (_socket.connect(IP, PORT) == sf::Socket::Done) {
    sf::Packet id_packet;
    id_packet << id;
    _socket.send(id_packet);
    _is_connected = true;
    std::cout << "Connection success" << std::endl;
  } else {
    std::cout << "Connection failure" << std::endl;
  }

  // listen to server in a separate thread
  std::thread listening_thread([this]() {
    while (_running) {
      std::cout << "Listening to server" << std::endl;
      sf::Packet id_packet;
      if (_socket.receive(id_packet) == sf::Socket::Done) {
        id_packet >> _input;
      } else {
        std::cout << "Disconnected from server" << std::endl;
        _running = false;
        break;
      }
      std::cout << "received from server: " << _input << std::endl;
    }
  });

  // Write to server in a separate thread on regular basis
  std::thread writing_thread([this]() {
    while (_running) {
      std::cout << "sending update to server" << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      std::lock_guard<std::mutex> lock(_buffer_mtx);
      if (_buffer.getDataSize() > 0) {
        auto status = _socket.send(_buffer);
        if (status != sf::Socket::Done) {
          std::cout << "Disconnected from server" << std::endl;
          _running = false;
          return;
        }
        _buffer.clear();
      } else {
        sf::Packet ping;
        ping << std::string("ping");
        auto status = _socket.send(ping);
        if (status != sf::Socket::Done) {
          return;
        }
      }
    }
  });

  // Cleanup
  listening_thread.join();
  writing_thread.join();
}

void Client::send(char cmd) {
  std::lock_guard<std::mutex> lock(_buffer_mtx);
  _buffer << cmd;
}

std::string Client::receive() {
  std::lock_guard<std::mutex> lock(_input_mtx);
  return _input;
}
