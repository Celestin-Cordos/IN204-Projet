#include <iostream>
#include "../include/gameClass.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

Piece::Piece(type typeP, color colorP)
{
    piece_type = typeP;
    piece_color = colorP;
}

Board::Board (int pas, int hauteur, int largeur)
{
    STEP = pas;
    HEIGHT = hauteur;
    WIDTH = largeur;
    color matrice [largeur*hauteur];
    for (int i = 0; i<hauteur*largeur; i++ )
    {
        matrice[i] = Black;
    }
    MATRIX=matrice;
}

Board::~Board()
{
    delete MATRIX;
    std::cout << "Board deleted succesflully" << std::endl;
}

