#include "network.hpp"
#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

const unsigned short PORT = 8080;
sf::IpAddress IP = sf::IpAddress::getLocalAddress();

Server::Server() { _listener.listen(PORT, IP); }

void Server::run() {

  int total_nb = 2;
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

    _clients.emplace_back(id, socket);
  }

  std::cout << "Connection success" << std::endl;

  // Start a thread by client to listen
  std::vector<std::thread> threads;
  for (auto client : _clients) {
    threads.emplace_back([client]() {
      while (true) {
        std::string input;
        { // Receive
          sf::Packet packet;
          auto status = client.second->receive(packet);
          if (status == sf::Socket::Done) {
            packet >> input;
          } else {
            std::cout << "Exiting" << std::endl;
            return;
          }
          std::cout << "Received from client " << client.first << ": " << input
                    << std::endl;

          if (input == "exit") {
            std::cout << "Client " << client.first << " disconnected"
                      << std::endl;
            return;
          }
        }

        // TODO: use input
        //

        { // Reply
          std::cout << "Sending update to client " << client.first << std::endl;
          sf::Packet board;
          board << "this is the state of the game";
          auto status = client.second->send(board);
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
  std::cout << "You are connected as a Player" << std::endl;
}

void Client::run() {
  std::string id;
  std::cout << "Enter online id: ";
  std::cin >> id;
  if (_socket.connect(IP, PORT) == sf::Socket::Done) {
    sf::Packet id_packet;
    id_packet << id;
    _socket.send(id_packet);
    std::cout << "Connection success" << std::endl;
  } else {
    std::cout << "Connection failure" << std::endl;
  }

  // listen to server in a separate thread
  std::atomic_bool running(true);
  std::thread listening_thread([this, &running]() {
    while (running) {
      std::cout << "Listening to server" << std::endl;
      sf::Packet id_packet;
      std::string input;
      if (_socket.receive(id_packet) == sf::Socket::Done) {
        id_packet >> input;
      } else {
        std::cout << "Disconnected from server" << std::endl;
        running = false;
        break;
      }
      std::cout << "received from server: " << input << std::endl;
    }
  });

  // Write to server in a separate thread on regular basis
  sf::Packet buffer;
  std::mutex buffer_mtx;
  std::thread writing_thread([this, &running, &buffer, &buffer_mtx]() {
    while (running) {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      std::lock_guard<std::mutex> lock(buffer_mtx);
      if (buffer.getDataSize() > 0) {
        auto status = _socket.send(buffer);
        if (status != sf::Socket::Done) {
          std::cout << "Disconnected from server" << std::endl;
          running = false;
          return;
        }
        buffer.clear();
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

  // get keyboard input
  while (running) {
    std::string input;
    std::cout << "\nEnter \"exit\" to quit or message to send: ";
    getline(std::cin, input);
    if (!input.size())
      continue;

    // Send input  to server (even last msg)
    {
      std::lock_guard<std::mutex> lock(buffer_mtx);
      buffer << input;
    }

    if (input == "exit") {
      std::cout << "Bye bye" << std::endl;
      running = false;
      break;
    }
  }

  // Cleanup
  running = false;
  listening_thread.join();
  writing_thread.join();
}