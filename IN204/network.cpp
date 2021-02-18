#include "network.hpp"
#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

const unsigned short PORT = 8080;
sf::IpAddress IP = sf::IpAddress::getLocalAddress();

Server::Server() { _listener.listen(PORT, IP); }

sf::Color correspondance_couleurs_sfml [] = {sf::Color::Black, sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan};

// en pixels:
int HEIGHT = 25; // en cases
int WIDTH = 15;
int STEP = 30;
int TIME_STEP = 45; // au bout de combien de temps on parcours un pas d'espace; a noter que c'est en frames et depend du nombre de fps de la fenetre


int nb_joueurs = 1;
void Server::run() {
    std::srand(std::time(nullptr));
    Game MaPartie (WIDTH, HEIGHT, STEP, nb_joueurs);
//    std::cout << "Debut d'affichage" << std::endl;
//    for (int i = 0; i<WIDTH; i++ )
//    {
//        for (int j = 0; j<HEIGHT; j++)
//            std::cout << i << ", " << j<< ":" << Monde.get_case(i,j)<< std::endl;
//
//    }

    std::list <enum commandes> commandes_attente;
    enum commandes* tableau_commandes[nb_joueurs]






      int total_nb = nb_joueurs;
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






    int i = 0;

    while (!MaPartie.game_over)
    {
      // Start a thread by client to listen
      std::vector<std::thread> threads;
      for (auto client : _clients) {
        threads.emplace_back([client]() {

            std::vector<int>  {grande_matrice_envoi};
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

        });
      }

      for (auto &thread : threads) {
        thread.join();
      }





        //mettre un sleep ici qui corresponde a la vitesse des deux fenetres
        i++;
        //std::cout << "Frame"<< std::endl;

        // avancement
        //std::cout << i <<"; "<< TIME_STEP << std::endl;
        for (int compteur = 0; compteur < MaPartie.Jeux.size())
        {

            enum commandes current = tableau_commandes[compteur];
            std::vector<int> matrice_envoi = {grande_matrice_envoi.begin()+compteur*WIDTH*HEIGHT, grande_matrice_envoi.begin()+(compteur+1)*WIDTH*HEIGHT-1}
            auto Monde = MaPartie.Jeux[compteur]; // un pointeur, donc
            Monde->executer_commandes ();
            if (i%TIME_STEP == 0)
            {
                MaPartie.continuer ();
            }

            //enregistrement des commandes
            Monde->commandes_recues.push_back (current);



            int n ;

            for (int i = 0; i < WIDTH; i++)
            {
                for (int j = 0; j <HEIGHT; j++)
                {
    //                for (int i = 0; i<WIDTH; i++ )
    //                {
    //                    for (int j = 0; j<HEIGHT; j++)
    //                        std::cout << i << ", " << j<< ":" << Monde.get_case(i,j)<< std::endl;
    //
    //                }
    //                exit (-1);
                    int couleur = Monde->get_case(i,j);

                    matrice_envoi[j*WIDTH+i] = couleur;

                }
            }
            //std::cout << "here" << std::endl; exit (-1);

            n = Monde->current_piece->taille_matrice;
            //std::cout << "current piece: " << std::endl; exit (-1);
            //std::cout << Monde.current_piece->pos_premiere_case[0] << Monde.current_piece->pos_premiere_case[1] << std::endl;
            int x = Monde->current_piece->pos_premiere_case[0];
            int y = Monde->current_piece->pos_premiere_case[1];
    //        std::cout << x << y << std::endl;
    //        std::cout << "here" << std::endl;
    //        exit (-1);
            int color = Monde->current_piece->get_color ();
            for (int i = 0; i<n ; i++)
            {
                for (int j = 0; j<n ; j++)
                {
                    if (Monde->current_piece->matrice[n*j +i] == 1)
                    {
                        matrice_envoi[WIDTH*j+i] = color;


                        //std::cout << i <<", "<<j<< ", "<<Monde.current_piece->matrice[n*j+i] <<std::endl;
                    }
                }

            }


        }

    }






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

    //// MAIN PART //////

    std::srand(std::time(nullptr));
    Game MaPartie (WIDTH, HEIGHT, STEP, 1);
//    std::cout << "Debut d'affichage" << std::endl;
//    for (int i = 0; i<WIDTH; i++ )
//    {
//        for (int j = 0; j<HEIGHT; j++)
//            std::cout << i << ", " << j<< ":" << Monde.get_case(i,j)<< std::endl;
//
//    }
    sf::RenderWindow window(sf::VideoMode(WIDTH*STEP, STEP*HEIGHT), "Tetris");
    window.setPosition (sf::Vector2i(0, 0));
    window.setFramerateLimit (40);
    std::list <enum commandes> commandes_attente;
    enum commandes current;
    enum commandes previous = pause_commande;

    auto Monde = MaPartie.Jeux[0]; // un pointeur, donc
    int i = 0;
    while (window.isOpen())
    {
        std::vector<int> matrice_reception (HEIGHT*WIDTH);
        int input  ;//pause sera une commande en soi
        input.push_back(1);// pas de pause
        i++;
        //std::cout << "Frame"<< std::endl;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // avancement
        //std::cout << i <<"; "<< TIME_STEP << std::endl;
        Monde->executer_commandes ();
        if (i%TIME_STEP == 0)
        {
            MaPartie.continuer ();
        }

        //enregistrement des commandes

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
             current = move_right;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            current = move_left;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            current = move_down;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            current = rotate_direct;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            current = rotate_undirect;
        }
        else
        {
            current = pause_commande;
        }


        //std::cout << "mouvement : " << current <<", " << previous <<std::endl;
        if (current!=previous)
        {
            input = current;
        }
        previous = current;
        //std::cout << "mouvement : " << current <<", " << previous <<std::endl;

        //// donc on envoie aussi pause_move... mais ca ne fait rien..

        //premiere version...fonctionnait quand on envoyait les commandes lors de l'avancement
//        if (current == pause_commande)// on envoie ce qu'il y a avant
//        {
//            enum commandes a_envoyer = *commandes_attente.begin ();
//            Monde->commandes_recues.push_back (a_envoyer);
//            while (*commandes_attente.begin()==a_envoyer && current!= pause_commande)
//            {
//                commandes_attente.pop_front (); // on nettoie les appuis 'involontaires'
//            }
//            while (commandes_attente.begin()!= commandes_attente.end())
//            {
//                commandes_attente.pop_front (); // et le dernier 'pause_command'
//            }
//        }
        //perception de l'�tat du monde et //affichage:

        // listen to server in a separate thread
          std::atomic_bool running(true);
          std::thread listening_thread([this, &running]()
        {
              std::cout << "Listening to server" << std::endl;
              sf::Packet id_packet;

              if (_socket.receive(id_packet) == sf::Socket::Done) {
                id_packet >> matrice_reception;
              } else {
                std::cout << "Disconnected from server" << std::endl;
                running = false;
                break;
              }
              std::cout << "received from server: "  << std::endl;

          });

          sf::Packet buffer;
          buffer << input;
          std::mutex buffer_mtx;
          std::thread writing_thread([this, &running, &buffer, &buffer_mtx]() {
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

          });



        listening_thread.join();
        writing_thread.join();

        window.clear();
        if (!MaPartie.game_over)
        {
            int n ;

            for (int i = 0; i < WIDTH; i++)
            {
                for (int j = 0; j <HEIGHT; j++)
                {
                    sf::RectangleShape rectangle (sf::Vector2f(STEP,STEP));
    //                for (int i = 0; i<WIDTH; i++ )
    //                {
    //                    for (int j = 0; j<HEIGHT; j++)
    //                        std::cout << i << ", " << j<< ":" << Monde.get_case(i,j)<< std::endl;
    //
    //                }
    //                exit (-1);
                    auto couleur = matrice_reception(j*WIDTH +i );
                    //std::cout <<  WIDTH*j+i<< "/" <<WIDTH*HEIGHT <<": " <<couleur << std::endl;

                    if (couleur != Black)
                    {
                        rectangle.setFillColor (correspondance_couleurs_sfml[couleur]);
                        rectangle.setOutlineColor (sf::Color::White);
                        rectangle.setOutlineThickness(1);
                        rectangle.setPosition(STEP*i,STEP*j);
                        window.draw(rectangle);
                    }

                }
            }
            //std::cout << "here" << std::endl; exit (-1);

            n = Monde->current_piece->taille_matrice;
            //std::cout << "current piece: " << std::endl; exit (-1);
            //std::cout << Monde.current_piece->pos_premiere_case[0] << Monde.current_piece->pos_premiere_case[1] << std::endl;
            int x = Monde->current_piece->pos_premiere_case[0];
            int y = Monde->current_piece->pos_premiere_case[1];
    //        std::cout << x << y << std::endl;
    //        std::cout << "here" << std::endl;
    //        exit (-1);


        window.display();
//        std::cout<< n << "sorti " <<std::endl; exit (-1);
    }


  // Write to server in a separate thread on regular basis


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

}
