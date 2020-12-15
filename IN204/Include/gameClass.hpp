
#ifndef GAMECLASS_HPP
#define GAMECLASS_HPP

#include <iostream>
#include <vector>
//#include "gameClass.hpp"

#include <SFML/Graphics.hpp>

using namespace std;

enum color {Black, Red, Green, Bleue, Yellow, Pink, Purple}; // Black est une case vide
enum type {I, O, T, L, J, Z, S};
enum direction {Left, Right};


class Piece
{
public:
	type piece_type;
	color piece_color;
	//list of positions (cases) it's filling in the matrix

	Piece();
	Piece(type typeP, color colorP);
	~Piece();

	void rotate(unsigned int deg);
	void move(direction dir);
};

class Board
{
    int pas; // taille d'une case (carrée)
    int HEIGHT; // en nombre de cases (pour pas voir de probleme d'arrondi en cas de divisions...)
    int WIDTH; // en nombre de cases (pour pas voir de probleme d'arrondi en cas de divisions...)
    color* MATRIX;//matrice de cases

    Board (int step, int hauteur, int largeur);


};

//class Game
//{
//public:
//	Case board[20][10];
//
//	Game();
//	~Game();
//};


#endif
