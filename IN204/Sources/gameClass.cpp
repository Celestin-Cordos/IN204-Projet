#include <iostream>
#include "../include/gameClass.hpp"
//#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>



using namespace std;


int myints1[] = {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0};
int myints2[] ={1,1,1,1};
int myints3[] ={0,1,0,1,1,1,0,0,0};
int myints4[] ={0,0,1,1,1,1,0,0,0};
int myints5[] ={1,0,0,1,1,1,0,0,0};
int myints6[] ={1,1,0,0,1,1,0,0,0};
int myints7[] ={0,1,1,1,1,0,0,0,0};// on parle bien (de la racine carrée) du nb de cases de la petite matrice qui contient la piece;

std::vector<int> vect1 (myints1, myints1+ 16);
std::vector<int> vect2 (myints2, myints2+ 4);
std::vector<int> vect3 (myints3, myints3+ 9);
std::vector<int> vect4 (myints4, myints4+ 9);
std::vector<int> vect5 (myints5, myints5+ 9);
std::vector<int> vect6 (myints6, myints6+ 9);
std::vector<int> vect7 (myints7, myints7+ 9);

std::vector<std::vector <int>> correspondances_forme_matrice {vect1, vect2,vect3,vect4,vect5,vect6,vect7};

int correspondances_forme_position[] = {-1,0,0,0,0,0,0}; // on ne met que le y
char* correspondance_couleurs[] = {"Black", "Red", "Green", "Blue", "Yellow", "Magenta", "Cyan"};







Piece::Piece(enum type typeP,enum color colorP, int x_premiere_case, int y_premiere_case)
{
	piece_color = colorP;
	pos_premiere_case = {x_premiere_case, y_premiere_case};
	matrice = correspondances_forme_matrice[typeP]; // petite matrice 3*3, 2*2 ou 4*4 contenant la forme// 1 = occupée, 0= non occupée
	taille_matrice = int (sqrt (matrice.size ())); // taille de la matrice
	rotation = 0; // appartient à |[0, 3]| la rotation réelle en sera un multiple: aide à repérer la forme dans le plan
    std::cout << "Piece created"<< std::endl;
}

Piece::Piece ()
{
    Piece (I, Red, 0,correspondances_forme_position[I]); // temporel// a changer
    std::cout << "Piece created"<< std::endl;
}

Piece::~Piece()
{
    std::cout<< "Piece deleted"<< std::endl;
}

void Piece::incorporate ()
{
    int len = taille_matrice;
    for (int i = 0; i< len; i++)
    {
        for (int j = 0; j< len; j++)
        {
            monde->append_case(i + pos_premiere_case[0], j=pos_premiere_case[1], piece_color);
        }

    }
    monde->accept ();
}
bool Piece::move(enum direction dir)
{
    int n = monde->WIDTH;
    bool permission = true; // = feasable
    int len = taille_matrice ;
    int X, Y; // de combien on bouge dans chaque direction
    auto matrice_temp = matrice;
    switch (dir)
    {
        case Up : X = 0; Y = -1; break;
        case Down : X = 0; Y = 1; break;
        case Left : X = -1; Y = 0; break;
        case Right : X = -1; Y = 0; break;
    }
    pos_premiere_case[0]+=X; pos_premiere_case[1]+=Y;
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j<len; j++)
        {
            if ( matrice[len*i+j]==1 &&(monde->get_matrix()[n*(i+pos_premiere_case[0])+j + pos_premiere_case[1]]!=Black  || i+pos_premiere_case[0] >= n || i+pos_premiere_case[0] <0 ))
            {
                return false;
            }
        }
    }
    matrice = matrice_temp;
    return permission;

}

void Piece::rotate (int quart_tour)
{
    bool continuation = true;
    if (quart_tour >=0)
    {
        while (quart_tour>0 && continuation)
        {
            continuation = elementary_rotate ();
            quart_tour--;
        }
    }
    else
    {
        while (quart_tour<0 && continuation)
        {
            continuation = elementary_unrotate ();
            quart_tour++;
        }
    }

}

bool Piece::elementary_rotate()
{
    int n = monde->WIDTH;
    bool permission = true; // = feasable
    int len = taille_matrice;
    auto matrice_temp = matrice;
    for (int i = 0; i< taille_matrice; i++)
    {
        for (int j = 0; j< taille_matrice; j++)
        {
            matrice_temp [taille_matrice*i+j] = matrice[taille_matrice*(taille_matrice-1-j) + i];
            if (pos_premiere_case[0] +i< n && pos_premiere_case[0] +i>=0) // si la case appartient à la matrice
            {
                if (monde->get_matrix()[taille_matrice*(pos_premiere_case[0] +i)+(pos_premiere_case[1]+j)]==1&&matrice_temp[taille_matrice*i+j] == 1)
                {
                    return false;
                }
            }
            else // si elle n'y appartient pas... il faut que la case de la petite matrice ne soit pas occupée
            {
                if (matrice_temp [taille_matrice*i+j]==1)
                {
                    return false;
                }
            }

        }
    }
    rotation++;
    return permission;


}

bool Piece::elementary_unrotate()
{
    int n = monde->WIDTH;
    bool permission = true; // = feasable
    int len = taille_matrice;
    auto matrice_temp = matrice;
    for (int i = 0; i< taille_matrice; i++)
    {
        for (int j = 0; j< taille_matrice; j++)
        {
            matrice_temp[taille_matrice*(taille_matrice-1-j) + i] = matrice[taille_matrice*i+j] ;
            if (pos_premiere_case[0] +i< n && pos_premiere_case[0] +i>=0) // si la case appartient à la matrice
            {
                if (monde->get_matrix()[taille_matrice*(pos_premiere_case[0] +i)+(pos_premiere_case[1]+j)]==1&&matrice_temp[taille_matrice*i+j] == 1)
                {
                    return false;
                }
            }
            else // si elle n'y appartient pas... il faut que la case de la petite matrice ne soit pas occupée
            {
                if (matrice_temp [taille_matrice*i+j]==1)
                {
                    return false;
                }
            }

        }
    }
    rotation--;
    return permission;

}




Board::Board (int pas, int hauteur, int largeur)
{
    STEP = pas;
    HEIGHT = hauteur;
    WIDTH = largeur;
    int taille_matrice = largeur*hauteur;
    enum color* matrice = new enum color [taille_matrice];
    for (int i = 0; i<largeur; i++ )
    {
        for (int j = 0; j<hauteur; j++)
            matrice[j*WIDTH+i] = Black;
    }
    MATRIX=matrice;
//    for (int i = 0; i<largeur; i++ )
//    {
//        for (int j = 0; j<hauteur; j++ )
//        {
//            std::cout << i<< "," << j<< ":" << MATRIX[j*WIDTH+i]<< std::endl;
//        }
//    }
    std::cout << "Matrice de jeu initialisee"<< std::endl;
}

Board::~Board()
{
    delete MATRIX;
    std::cout << "Board deleted succesflully" << std::endl;
}

enum color Board::get_case (int i, int j)
{
    return MATRIX[WIDTH*j+i];
}

void Board::append_case (int i, int j, enum color couleur)
{
    MATRIX[WIDTH*j+i] = couleur;
}



void Board::nouvelle_piece ()// pour l'instant, on n'ajoute que des cubes
{
    Piece *piece = new Piece (I, Red, 0,correspondances_forme_position[I]);// temporaire
    current_piece = piece;
    piece->monde = this;
}

void Board::accept()
{
    delete current_piece;
    nouvelle_piece ();
}

enum color* Board::get_matrix ()
{
    return (MATRIX);
}
