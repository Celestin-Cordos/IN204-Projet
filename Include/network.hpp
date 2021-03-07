#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "gameClass.hpp"
#include <SFML/Network.hpp>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>

using socket_t = std::shared_ptr<sf::TcpSocket>;

struct Connexion
{
  int _width;
  int _height;
  std::size_t id;
  std::string name;
  socket_t socket;
  commandes input;
  std::string state;
  std::vector<int> buffer_envoi;

  Connexion(std::size_t id, std::string const &name, socket_t socket, int width, int height);
};

class Server
{
public:
  using client_t = std::pair<std::string, socket_t>;
  int _width;
  int _height;
  sf::TcpListener _listener;
  std::vector<Connexion> _clients;

  std::atomic_bool _running;

  std::mutex _mtx;
  int _nb_clients;

public:
  //Server();
  Server(int width, int height, int nb_joueurs);

  void run();

  std::vector<commandes> receive();

  void send(std::vector<int> tableau, int id_client);

  std::size_t numberOfConnection() const { return _clients.size(); }
};

class Client
{
public:
  int _height;
  int _width;
  sf::TcpSocket _socket;
  sf::Packet _buffer;
  std::mutex _buffer_mtx;
  std::atomic_bool _running;
  std::atomic_bool _is_connected;
  std::string _input;
  std::mutex _input_mtx;
  std::vector<int> buffer_reception;

public:
  Client(int width, int height);

  void run();

  void send(commandes cmd);

  void stop() { _running = false; }

  std::vector<int> receive();

  bool connected() const { return _is_connected; }
};

#endif