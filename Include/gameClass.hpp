#ifndef GAMECLASS_HPP
#define GAMECLASS_HPP

#include <iostream>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>

using namespace std;

enum color
{
    Black,
    Red,
    Green,
    Blue,
    Yellow,
    Magenta,
    Cyan
}; // Black est une case vide

//int example [] = {}

enum type
{
    I,
    O,
    T,
    L,
    J,
    Z,
    S
};
enum direction
{
    Left,
    Right,
    Up,
    Down
};
enum commandes
{
    random_noise,
    move_left,
    move_right,
    rotate_direct,
    rotate_undirect,
    pause_commande,
    move_down,
    ping,
    byebye,
    none
}; // commandes possibles recues par le Board// pause n'est pas vraiment recue, mais elle est enregistree. il faut deux commandes d'un m�me type suivies d'une pause pour les envoyer toutes les deux

class Piece;
class Game;

class Board // le point (0,0) est en haut a gauche// correspond � un 'ecran' ie le jeu d'un joueur
{
public:
    int nr_joueur;
    Game *Partie;
    bool board_over;    //=false if game is on going
    int STEP;           // taille d'une case (carr�e)
    int HEIGHT;         // en nombre de cases (pour pas voir de probleme d'arrondi en cas de divisions...)
    int WIDTH;          // en nombre de cases (pour pas voir de probleme d'arrondi en cas de divisions...)
    enum color *MATRIX; //matrice de cases color�es
    Piece *current_piece;
    std::list<enum type> pieces_suivantes; //ainsi, on peut faire pop_front en temps constant en evitant les copies;
    std::list<enum commandes> commandes_recues;

    Board(int step, int hauteur, int largeur, enum type pieceT, int numero_joueur);
    ~Board();

    enum color get_case(int i, int j);
    void append_case(int i, int j, enum color couleur);
    void accept(); // incorpore la piece
    void nouvelle_piece();
    enum color *get_matrix();
    int get_matrix_size();
    void continuer();          // fait avancer le plateau de jeu: fait tomber la piece, la fait tourner, et bouger; Elle est appel�e de l'ext�rieur uniquement aux bonnes �tapes
    void accepter_commandes(); // fait a chaque etape de la fenetre
    void executer_commandes(); // fait a chaque etape de la fenetre
    void change_piece();
    void append_piece(enum type);
    void check_lines(int x, int y, int n);
};

class Game // correspond a une partie
{
public:
    int WIDTH;
    int HEIGHT;
    int STEP;
    int joueurs;
    std::vector<int> score_joueurs;        //+1 a chaque ligne finie;
    bool game_over;                        //=false if game is on going
    std::vector<Board *> Jeux;             // une seule partie mais plusieurs joueurs
    std::list<enum type> pieces_suivantes; // contient les pieces a venir /// deux pour pouvoir allouer en asynchrone (event.) // les passe aux Board chaque fois que l'un d'entre eux en demande une.

    Game(int width, int height, int step, int nb_joueurs);
    ~Game();

    void nouvelle_piece(); // rajoute une nouvelle
    void broadcast_piece();
    void continuer();
    void stop_game(int numero_joueur);
};

class Piece
{
public:
    enum type piece_type;
    Board *monde;
    color piece_color;
    std::vector<int> pos_premiere_case; // {x,y}
    std::vector<int> matrice;           // petite matrice 3*3, 2*2 ou 4*4 contenant la forme// 1 = occup�e, 0= non occup�e
    int taille_matrice;                 // taille de la matrice
    int rotation;                       // appartient � |[0, 3]| la rotation r�elle en sera un multiple: aide � rep�rer la forme dans le plan

    Piece();
    Piece(enum type typeP, enum color colorP, int x_premiere_case, int y_premiere_case);
    ~Piece();

    bool elementary_rotate();
    bool elementary_unrotate();
    void rotate(int quart_tour); // donne le nb de quarts de tour
    bool move(enum direction dir);
    void incorporate();
    int get_case(int i, int j);
};

#endif
