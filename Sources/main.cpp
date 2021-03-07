#include "../Include/gameClass.hpp"
#include "../Include/network.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>

sf::Color correspondance_couleurs_sfml[] = {
    sf::Color::Black, sf::Color::Red, sf::Color::Green, sf::Color::Blue,
    sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan};

// en pixels:
int HEIGHT = 25; // en cases
int WIDTH = 15;
int STEP = 30;
int TIME_STEP = 240; //ca va etre en nb d'etapes// une etape =~ 1/120 s
int NB_JOUEURS = 2;

// Il faut donc initialiser les fenetres avec le meme nombre de fps

void MainServeur()
{
  Server network_server(WIDTH, HEIGHT, NB_JOUEURS);

  std::thread writing_thread([&network_server]() { network_server.run(); });

  while (network_server.numberOfConnection() != NB_JOUEURS)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  std::srand(std::time(nullptr));
  Game MaPartie(WIDTH, HEIGHT, STEP, NB_JOUEURS);

  // un pointeur, donc
  int i = 0;
  while (!MaPartie.game_over)
  {
    i++;
    for (int compteur_joueur = 0; compteur_joueur < NB_JOUEURS; compteur_joueur++)
    {
      auto Monde = MaPartie.Jeux[compteur_joueur];
      // TODO: receive cmd from client
      auto cmd = network_server.receive()[compteur_joueur];
      //std::cout << "Commande recue par le serveur : " << cmd << std::endl;
      if (cmd != none)
        Monde->commandes_recues.push_back(cmd);

      Monde->executer_commandes();
      if (i % TIME_STEP == 0)
      {
        MaPartie.continuer();
      }

      //std::cout << std::endl;
      std::vector<int> state;
      for (int j = 0; j < Monde->HEIGHT; j++)
      {
        for (int i = 0; i < Monde->WIDTH; i++)
        {
          int couleur = Monde->get_case(i, j);
          state.push_back(couleur);
          //std::cout << couleur;
        }
        //std::cout << std::endl;
      }

      int n = Monde->current_piece->taille_matrice;
      int x = Monde->current_piece->pos_premiere_case[0];
      int y = Monde->current_piece->pos_premiere_case[1];
      for (int i = 0; i < n; i++)
      {
        for (int j = 0; j < n; j++)
        {
          if (Monde->current_piece->matrice[n * j + i] == 1)
          {
            state[((i + x) + WIDTH * (j + y))] =
                Monde->current_piece->piece_color;
          }
        }
      }

      network_server.send(state, compteur_joueur);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(8));
  }

  std::cout << "Game is over" << std::endl;
}

void MainClient()
{
  Client network_client(WIDTH, HEIGHT);

  std::thread writing_thread([&network_client]() { network_client.run(); });

  while (!network_client.connected())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // load game sounds
  sf::Music startmusic;
  startmusic.openFromFile("../sounds/music.wav");
  startmusic.setVolume(16.f);
  startmusic.setLoop(true);
  startmusic.play();
  // load rotation music
  sf::SoundBuffer rot_buffer;
  rot_buffer.loadFromFile("../sounds/block-rotate.wav");
  sf::Sound rotate_sound;
  rotate_sound.setBuffer(rot_buffer);
  rotate_sound.setVolume(20.f);
  // load move music
  sf::SoundBuffer move_buffer;
  move_buffer.loadFromFile("../sounds/selection.wav");
  sf::Sound move_sound;
  move_sound.setBuffer(move_buffer);
  move_sound.setVolume(20.f);
  // load fall music
  sf::SoundBuffer fall_buffer;
  fall_buffer.loadFromFile("../sounds/fall.wav");
  sf::Sound fall_sound;
  fall_sound.setBuffer(fall_buffer);
  fall_sound.setVolume(20.f);

  std::srand(std::time(nullptr));
  // Game MaPartie(WIDTH, HEIGHT, STEP, 1);
  sf::RenderWindow window(sf::VideoMode(WIDTH * STEP, STEP * HEIGHT), "Tetris");
  window.setPosition(sf::Vector2i(0, 0));
  window.setFramerateLimit(40);
  std::list<enum commandes> commandes_attente;

  // auto Monde = MaPartie.Jeux[0]; // un pointeur, donc
  int i = 0;
  while (window.isOpen())
  {
    enum commandes current = commandes::none;
    enum commandes previous = pause_commande;
    i++;
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        startmusic.stop();
        window.close();
      }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
      current = move_right;
      move_sound.play();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
      current = move_left;
      move_sound.play();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
      current = move_down;
      fall_sound.play();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
      current = rotate_direct;
      rotate_sound.play();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
      current = rotate_undirect;
      rotate_sound.play();
    }
    else
    {
      current = pause_commande;
    }

    if (current != previous)
    {
      //std::cout << "sending current: " << current << std::endl;
      network_client.send(current);
    }
    previous = current;
    window.clear();

    std::vector<int> board = network_client.receive();
    //std::cout << std::endl;
    for (int j = 0; j < HEIGHT; j++)
    {
      for (int i = 0; i < WIDTH; i++)
      {
        //std::cout << board[i + WIDTH * j];
      }
      //std::cout << std::endl;
    }

    int n;
    for (int i = 0; i < WIDTH; i++)
    {
      for (int j = 0; j < HEIGHT; j++)
      {
        sf::RectangleShape rectangle(sf::Vector2f(STEP, STEP));
        auto couleur = board[i + WIDTH * j];

        if (couleur != Black)
        {
          rectangle.setFillColor(correspondance_couleurs_sfml[couleur]);
          rectangle.setOutlineColor(sf::Color::White);
          rectangle.setOutlineThickness(1);
          rectangle.setPosition(STEP * i, STEP * j);
          window.draw(rectangle);
        }
        else
        {
          rectangle.setFillColor(sf::Color::Black);
          rectangle.setOutlineColor(sf::Color::Black);
          rectangle.setOutlineThickness(0);
          rectangle.setPosition(STEP * i, STEP * j);
          window.draw(rectangle);
        }
      }
    }
    window.display();
  }
}

int main(int argc, char *argv[])
{
  std::cout << "Welcome to Tetris" << std::endl;

  char who;
  std::cout << "Do you want to be a server (s) or a client (c) ? ";
  std::cin >> who;

  if (who == 's')
  {
    std::cout << "Creation of server" << std::endl;
    std::string password = " ";
    // ask for password to login as server
    while (password != "tetris")
    {
      std::cout << "Enter password : ";
      std::cin >> password;
      if (password != "tetris")
      {
        std::cout << "Wrong password! Please try again.\n"
                  << std::endl;
      }
      std::string difficulty = " ";
      while (difficulty != "easy" && difficulty != "medium" && difficulty != "hard")
      {
        std::cout << "Enter difficulty (easy|medium|hard) : ";
        std::cin >> difficulty;
        if (difficulty == "easy")
          TIME_STEP = 220;
        else if (difficulty == "medium")
          TIME_STEP = 100;
        else if (difficulty == "hard")
          TIME_STEP = 50;
      }
      std::cout << "Enter number of players : ";
      std::cin >> NB_JOUEURS;
    }

    MainServeur();
    return 0;
  }

  MainClient();
  return 0;
}
