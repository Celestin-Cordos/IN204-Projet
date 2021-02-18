#include "../Include/gameClass.hpp"
#include "network.hpp"
#include <SFML/Graphics.hpp>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>

sf::Color correspondance_couleurs_sfml[] = {
    sf::Color::Black,  sf::Color::Red,     sf::Color::Green, sf::Color::Blue,
    sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan};

// en pixels:
int HEIGHT = 25; // en cases
int WIDTH = 15;
int STEP = 30;
int TIME_STEP =
    45; // au bout de combien de temps on parcours un pas d'espace; a noter que
        // c'est en frames et depend du nombre de fps de la fenetre

// Il faut donc initialiser les fenetres avec le meme nombre de fps

void MainServeur() {
  Server network_server;
  // TODO: handle connection
  std::thread writing_thread([&network_server]() { network_server.run(); });

  std::srand(std::time(nullptr));
  Game MaPartie(WIDTH, HEIGHT, STEP, 1);

  auto Monde = MaPartie.Jeux[0]; // un pointeur, donc
  int i = 0;
  while (!MaPartie.game_over) {
    i++;

    // TODO: receive cmd from client
    auto cmd = network_server.receive()[0];
    Monde->commandes_recues.push_back(cmd);

    Monde->executer_commandes();
    if (i % TIME_STEP == 0) {
      MaPartie.continuer();
    }

    std::string state;
    for (int i = 0; i < WIDTH; i++) {
      for (int j = 0; j < HEIGHT; j++) {
        auto couleur = Monde->get_case(i, j);
        state.push_back(couleur);
      }
    }

    // TODO: send state
    network_server.send(state, 0);
  }
}

void MainClient() {
  Client network_client;

  // TODO: handle connection
  std::thread writing_thread([&network_client]() { network_client.run(); });

  std::srand(std::time(nullptr));
  Game MaPartie(WIDTH, HEIGHT, STEP, 1);
  sf::RenderWindow window(sf::VideoMode(WIDTH * STEP, STEP * HEIGHT), "Tetris");
  window.setPosition(sf::Vector2i(0, 0));
  window.setFramerateLimit(40);
  std::list<enum commandes> commandes_attente;
  enum commandes current;
  enum commandes previous = pause_commande;

  auto Monde = MaPartie.Jeux[0]; // un pointeur, donc
  int i = 0;
  while (window.isOpen()) {
    i++;
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    Monde->executer_commandes();
    if (i % TIME_STEP == 0) {
      MaPartie.continuer();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      current = move_right;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      current = move_left;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      current = move_down;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
      current = rotate_direct;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      current = rotate_undirect;
    } else {
      current = pause_commande;
    }

    if (current != previous) {
      network_client.send(current);
    }
    previous = current;
    window.clear();

    std::string board = network_client.receive();
    if (!MaPartie.game_over) {
      int n;

      for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
          sf::RectangleShape rectangle(sf::Vector2f(STEP, STEP));
          auto couleur = board[i + WIDTH * j];

          if (couleur != Black) {
            rectangle.setFillColor(correspondance_couleurs_sfml[couleur]);
            rectangle.setOutlineColor(sf::Color::White);
            rectangle.setOutlineThickness(1);
            rectangle.setPosition(STEP * i, STEP * j);
            window.draw(rectangle);
          }
        }
      }
      n = Monde->current_piece->taille_matrice;
      int x = Monde->current_piece->pos_premiere_case[0];
      int y = Monde->current_piece->pos_premiere_case[1];

      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (Monde->current_piece->matrice[n * j + i] == 1) {
            sf::RectangleShape rectangle(sf::Vector2f(STEP, STEP));
            rectangle.setFillColor(
                correspondance_couleurs_sfml[Monde->current_piece
                                                 ->piece_color]);
            rectangle.setPosition(STEP * (i + x), STEP * (j + y));
            window.draw(rectangle);
          }
        }
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(STEP * (i + x), 0)),
            sf::Vertex(sf::Vector2f(STEP * (i + x), STEP * HEIGHT))};

        window.draw(line, 2, sf::Lines);
      }
    }
    window.display();
  }
}

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

    MainServeur();
    return 0;
  }

  MainClient();
  return 0;
}