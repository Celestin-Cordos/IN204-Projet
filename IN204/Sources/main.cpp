#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include "../Include/gameClass.hpp"

sf::Color correspondance_couleurs_sfml [] = {sf::Color::Black, sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan};

// en pixels:
int HEIGHT = 25; // en cases
int WIDTH = 15;
int STEP = 30;
int TIME_STEP = 45; // au bout de combien de temps on parcours un pas d'espace; a noter que c'est en frames et depend du nombre de fps de la fenetre

// Il faut donc initialiser les fenetres avec le meme nombre de fps

int main()
{
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
            Monde->commandes_recues.push_back (current);
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
                    auto couleur = Monde->get_case(i,j);
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

            for (int i = 0; i<n ; i++)
            {
                for (int j = 0; j<n ; j++)
                {
                    if (Monde->current_piece->matrice[n*j +i] == 1)
                    {
                        sf::RectangleShape rectangle (sf::Vector2f(STEP,STEP));
                        rectangle.setFillColor (correspondance_couleurs_sfml[Monde->current_piece->piece_color]);
                        rectangle.setPosition(STEP*(i + x),STEP*(j+y));
                        window.draw(rectangle);


                        //std::cout << i <<", "<<j<< ", "<<Monde.current_piece->matrice[n*j+i] <<std::endl;
                    }
                }
                        sf::Vertex line[] =
                        {
                            sf::Vertex(sf::Vector2f(STEP*(i + x),0)),
                            sf::Vertex(sf::Vector2f(STEP*(i + x),STEP*HEIGHT))
                        };

                        window.draw(line, 2, sf::Lines);
            }

        }
        window.display();
//        std::cout<< n << "sorti " <<std::endl; exit (-1);
    }

    return 0;
}
