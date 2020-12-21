#ifndef GAMECLASS_HPP
#define GAMECLASS_HPP

#include <iostream>
#include <vector>
//#include "gameClass.hpp"

#include <SFML/Graphics.hpp>

using namespace std;

enum color {Black, Red, Green, Blue, Yellow, Magenta, Cyan}; // Black est une case vide

//int example [] = {}

enum type {I, O, T, L, J, Z, S};
enum direction {Left, Right, Up, Down};

class Piece ;

class Board // le point (0,0) est en haut a gauche
{
public:
    int STEP; // taille d'une case (carrée)
    int HEIGHT; // en nombre de cases (pour pas voir de probleme d'arrondi en cas de divisions...)
    int WIDTH; // en nombre de cases (pour pas voir de probleme d'arrondi en cas de divisions...)
    enum color* MATRIX;//matrice de cases colorées
    Piece* current_piece;

    Board (int step, int hauteur, int largeur);
    ~Board ();

    enum color get_case(int i, int j);
    void append_case (int i, int j, enum color couleur);
    void accept();// incorpore la piece
    void nouvelle_piece ();
    enum color* get_matrix ();
    int get_matrix_size ();


};

class Game  // correspond a une partie
{
    Board* Monde;
    float vitesse ; // en steeps par seconde;
    int joueurs;

};

class Piece
{
public:
	//type piece_type;
	Board* monde;
	color piece_color;
	std::vector <int> pos_premiere_case; // {x,y}
	std::vector <int> matrice; // petite matrice 3*3, 2*2 ou 4*4 contenant la forme// 1 = occupée, 0= non occupée
	int taille_matrice; // taille de la matrice
	int rotation; // appartient à |[0, 3]| la rotation réelle en sera un multiple: aide à repérer la forme dans le plan


	Piece();
	Piece(enum type typeP,enum color colorP, int x_premiere_case, int y_premiere_case);
	~Piece();

	bool elementary_rotate ();
    bool elementary_unrotate ();
	void rotate(int quart_tour); // donne le nb de quarts de tour
	bool move(enum direction dir);
	void incorporate ();
	nouvelle_piece ();
};




#endif
