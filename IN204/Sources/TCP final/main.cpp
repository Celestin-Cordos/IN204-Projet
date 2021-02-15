#include "network.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::cout << "Bienvenue dans le test de communication de Tetris" << std::endl;

  char who;
  std::cout << "Do you want to be a server (s) or a client (c) ? ";
  std::cin >> who;

  if (who == 's') {
    std::cout << "Creation d'un serveur" << std::endl;
    std::string password = " ";
    // ask for password to login as server
    while (password != "tetris") {
      std::cout << "Enter password : ";
      std::cin >> password;
      if (password != "tetris") {
        std::cout << "Wrong password! Please try again.\n" << std::endl;
      }
    }

    Server server;
    server.run();
    return 0;
  }

  Client client;
  client.run();

  return 0;
}