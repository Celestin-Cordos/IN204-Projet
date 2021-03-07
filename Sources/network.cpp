#include "../Include/network.hpp"
#include <algorithm>
#include <atomic>
#include <iostream>
#include <iterator>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

const unsigned short PORT = 8080;
sf::IpAddress IP = sf::IpAddress::getLocalAddress();

vector<int> strToVect(string s)
{
  vector<int> vect;
  std::stringstream oss;
  for (char c : s)
  {
    if (c == '0')
      vect.push_back(0);
    else if (c == '1')
      vect.push_back(1);
  }
  return vect;
}

string vectToStr(vector<int> vec)
{
  std::ostringstream oss;

  if (!vec.empty())
  {
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(oss, ""));
  }

  return oss.str();
}

Server::Server(int width, int height, int nb_joueurs)
{
  _width = width;
  _height = height;
  _listener.listen(PORT, IP);
  _running = false;
  _nb_clients = nb_joueurs;
}

std::vector<commandes> Server::receive()
{
  std::vector<commandes> result(_clients.size(), ping);
  // std::lock_guard<std::mutex> lock(_mtx);
  for (auto &client : _clients)
  {
    result[client.id] = client.input;
    client.input = none;
  }
  return result;
}

void Server::send(std::vector<int> tableau, int id_client)
{
  // std::lock_guard_server<std::mutex> lock(buffer_envoi_mutex);
  _clients[id_client].buffer_envoi = tableau;
}

void Server::run()
{
  _running = true;

  int total_nb = _nb_clients;
  std::cout << "waiting for " << total_nb << " client(s)" << std::endl;
  while (_clients.size() < total_nb)
  {
    std::cout << "Waiting for client" << std::endl;

    auto socket = std::make_shared<sf::TcpSocket>();
    auto status = _listener.accept(*socket);
    if (status != sf::Socket::Done)
    {
      continue;
    }

    sf::Packet id_packet;
    std::string id;
    if (socket->receive(id_packet) == sf::Socket::Done)
    {
      id_packet >> id;
    }
    else
    {
      continue;
    }
    std::cout << "Player " << id << " is connected (" << _clients.size() << "/"
              << total_nb << ") - IP : " << socket->getRemoteAddress()
              << std::endl;

    _clients.emplace_back(_clients.size(), id, socket, _width, _height);
  }

  std::cout << "Connection success" << std::endl;

  // Start a thread by client to listen
  std::vector<std::thread> threads;
  for (auto &client : _clients)
  {
    threads.emplace_back([this, &client]() {
      while (_running)
      {
        int input;
        { // Receive
          sf::Packet packet;
          auto status = client.socket->receive(packet);
          if (status == sf::Socket::Done)
          {
            packet >> input;
          }
          else
          {
            std::cout << "Exiting" << std::endl;
            return;
          }

          if (input == commandes::byebye)
          {
            std::cout << "Client " << client.id << " disconnected" << std::endl;
            return;
          }
        }

        sf::Packet board;
        {
          std::string message = vectToStr(client.buffer_envoi);
          if (input != commandes::ping)
            client.input = static_cast<commandes>(input);
          board << message;
        }

        { // Reply
          auto status = client.socket->send(board);
          if (status != sf::Socket::Done)
          {
            std::cout << "Disconnected from client" << std::endl;
            return;
          }
        }
      }
    });
  }

  for (auto &thread : threads)
  {
    thread.join();
  }
  std::cout << "Communication ended" << std::endl;
}

Client::Client(int width, int height)
{
  _socket.connect(IP, PORT);
  _running = false;
  _is_connected = false;
  _height = height;
  _width = width;
  std::cout << "You are connected as a Player" << std::endl;
  for (int i = 0; i < _width * _height; i++)
    buffer_reception.push_back(0);
}

void Client::run()
{
  _running = true;
  std::string id;
  std::cout << "Enter online id: ";
  std::cin >> id;
  if (_socket.connect(IP, PORT) == sf::Socket::Done)
  {
    sf::Packet id_packet;
    id_packet << id;
    _socket.send(id_packet);
    _is_connected = true;
    std::cout << "Connection success" << std::endl;
  }
  else
  {
    std::cout << "Connection failure" << std::endl;
  }

  // listen to server in a separate thread
  std::thread listening_thread([this]() {
    while (_running)
    {
      sf::Packet id_packet;
      if (_socket.receive(id_packet) == sf::Socket::Done)
      {
        id_packet >> _input;
      }
      else
      {
        std::cout << "Disconnected from server" << std::endl;
        _running = false;
        break;
      }
      buffer_reception = strToVect(_input);
    }
  });

  // Write to server in a separate thread on regular basis
  std::thread writing_thread([this]() {
    while (_running)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      // std::lock_guard<std::mutex> lock(_buffer_mtx);
      if (_buffer.getDataSize() > 0)
      {
        auto status = _socket.send(_buffer);
        if (status != sf::Socket::Done)
        {
          std::cout << "Disconnected from server" << std::endl;
          _running = false;
          return;
        }
        _buffer.clear();
      }
      else
      {
        sf::Packet ping;
        ping << commandes::ping;
        auto status = _socket.send(ping);
        if (status != sf::Socket::Done)
        {
          return;
        }
      }
    }
  });

  // Cleanup
  listening_thread.join();
  writing_thread.join();
}

void Client::send(commandes cmd)
{
  std::lock_guard<std::mutex> lock(_buffer_mtx);
  _buffer.clear();
  _buffer << cmd;
}

std::vector<int> Client::receive()
{
  // std::lock_guard<std::mutex> lock(_input_mtx);
  return buffer_reception;
}

Connexion::Connexion(std::size_t id, std::string const &name, socket_t socket,
                     int width, int height)
{
  _width = width;
  _height = height;
  this->id = id;
  this->name = name;
  this->socket = socket;
  for (int i = 0; i < _height * _width; i++)
    buffer_envoi.push_back(0);
}
