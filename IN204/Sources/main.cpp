#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Include/gameClass.hpp"

sf::Color correspondance_couleurs_sfml [] = {sf::Color::Black, sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan};

// en pixels:
int HEIGHT = 40; // en cases
int WIDTH = 20;
int STEP = 20;

int main()
{
    Board Monde (STEP, HEIGHT, WIDTH);
    Monde.nouvelle_piece ();
    std::cout << "Debut d'affichage" << std::endl;
    for (int i = 0; i<WIDTH; i++ )
    {
        for (int j = 0; j<HEIGHT; j++)
            std::cout << i << ", " << j<< ":" << Monde.get_case(i,j)<< std::endl;

    }
    sf::RenderWindow window(sf::VideoMode(WIDTH*STEP, STEP*HEIGHT), "Tetris");
    window.setPosition (sf::Vector2i(0, 0));
    window.setFramerateLimit (60);


    int i = 0;
    while (window.isOpen())
    {
        i++;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // avancement

        // perception de l'état du monde et //affichage:
        window.clear();
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
                auto couleur = Monde.get_case(i,j);
                std::cout <<  WIDTH*j+i<< "/" <<WIDTH*HEIGHT <<": " <<couleur << std::endl;
                //std::cout<< Monde.MATRIX;

                if (couleur != Black)
                {
                    rectangle.setFillColor (correspondance_couleurs_sfml[couleur]);
                    rectangle.setPosition(STEP*i,STEP*j);
                    window.draw(rectangle);
                }

            }
        }
//        Piece piece(I, Red, 5,10);
//        std::cout << piece.pos_premiere_case[0] << piece.pos_premiere_case[1] << std::endl; exit (-1);

        n = Monde.current_piece->taille_matrice;
        std::cout << Monde.current_piece->pos_premiere_case[0] << Monde.current_piece->pos_premiere_case[1] << std::endl;
        int x = Monde.current_piece->pos_premiere_case[0];
        int y = Monde.current_piece->pos_premiere_case[1];
        std::cout << x << y << std::endl;
        //exit (-1);

        for (int i = 0; i<n ; i++)
        {
            for (int j = 0; j<n ; j++)
            {
                if (Monde.current_piece->matrice[n*j +i] == 1)
                {
                    sf::RectangleShape rectangle (sf::Vector2f(STEP,STEP));
                    rectangle.setFillColor (correspondance_couleurs_sfml[Monde.current_piece->piece_color]);
                    rectangle.setPosition(STEP*(i + x),STEP*(j+y));
                    window.draw(rectangle);
                    std::cout << i <<", "<<j<< ", "<<Monde.current_piece->matrice[n*j+i] <<std::endl;
                }
            }

        }

        window.display();
//        std::cout<< n << "sorti " <<std::endl; exit (-1);
    }

    return 0;
}
