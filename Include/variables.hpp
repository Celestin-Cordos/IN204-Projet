#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include <vector>

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
std::vector<int> vect1 = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
std::vector<int> vect2{1, 1, 1, 1};
std::vector<int> vect3{0, 1, 0, 1, 1, 1, 0, 0, 0};
std::vector<int> vect4{0, 0, 1, 1, 1, 1, 0, 0, 0};
std::vector<int> vect5{1, 0, 0, 1, 1, 1, 0, 0, 0};
std::vector<int> vect6{1, 1, 0, 0, 1, 1, 0, 0, 0};
std::vector<int> vect7{0, 1, 1, 1, 1, 0, 0, 0, 0}; // on parle bien (de la racine carr�e) du nb de cases de la petite matrice qui contient la piece;

std::vector<int> correspondances_forme_matrice[] = {vect1, vect2, vect3, vect4, vect5, vect6, vect7};

int correspondances_forme_position[] = {-1, 0, 0, 0, 0, 0, 0}; // on ne met que le y
char *correspondance_couleurs[] = {"Black", "Red", "Green", "Blue", "Yellow", "Magenta", "Cyan"};

#endif
