#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "gameClass.hpp"
#include <SFML/Network.hpp>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>

using socket_t = std::shared_ptr<sf::TcpSocket>;

struct Connexion {
  std::size_t id;
  std::string name;
  socket_t socket;
  std::string input;
  std::string state;

  Connexion(std::size_t id, std::string const &name, socket_t socket)
      : id(id), name(name), socket(socket) {}
};

class Server {
private:
  using client_t = std::pair<std::string, socket_t>;

  sf::TcpListener _listener;
  std::vector<Connexion> _clients;

  std::atomic_bool _running;

  std::mutex _mtx;

public:
  Server();

  void run();

  std::vector<commandes> receive();

  void send(std::string state, std::size_t id) {
    std::lock_guard<std::mutex> lock(_mtx);
    _clients[id].state = state;
  }
};

class Client {
private:
  sf::TcpSocket _socket;
  sf::Packet _buffer;
  std::mutex _buffer_mtx;
  std::atomic_bool _running;
  std::string _input;
  std::mutex _input_mtx;

public:
  Client();

  void run();

  void send(char cmd);

  void stop() { _running = false; }

  std::string receive();
};

#endif