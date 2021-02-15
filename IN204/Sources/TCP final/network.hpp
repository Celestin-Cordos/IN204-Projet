#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <SFML/Network.hpp>
#include <memory>
#include <string>

class Server {
private:
  using socket_t = std::shared_ptr<sf::TcpSocket>;
  using client_t = std::pair<std::string, socket_t>;

  sf::TcpListener _listener;
  std::vector<client_t> _clients;

  // to change for n player, now 2
  char modes[2];
  char buffer[10000];
  size_t received;

public:
  Server();

  void run();
};

class Client {
private:
  sf::TcpSocket _socket;
  char buffer[10000];
  size_t received;

public:
  Client();

  void run();
};

#endif