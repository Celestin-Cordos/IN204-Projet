#include "../Include/gameClass.hpp"
#include <iostream>

//#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

int myints1[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
int myints2[] = {1, 1, 1, 1};
int myints3[] = {0, 1, 0, 1, 1, 1, 0, 0, 0};
int myints4[] = {0, 0, 1, 1, 1, 1, 0, 0, 0};
int myints5[] = {1, 0, 0, 1, 1, 1, 0, 0, 0};
int myints6[] = {1, 1, 0, 0, 1, 1, 0, 0, 0};
int myints7[] = {
    0, 1, 1, 1, 1,
    0, 0, 0, 0}; // on parle bien (de la racine carr�e) du nb de cases de la
                 // petite matrice qui contient la piece;

std::vector<int> vect1(myints1, myints1 + 16);
std::vector<int> vect2(myints2, myints2 + 4);
std::vector<int> vect3(myints3, myints3 + 9);
std::vector<int> vect4(myints4, myints4 + 9);
std::vector<int> vect5(myints5, myints5 + 9);
std::vector<int> vect6(myints6, myints6 + 9);
std::vector<int> vect7(myints7, myints7 + 9);

std::vector<std::vector<int>> correspondances_forme_matrice{
    vect1, vect2, vect3, vect4, vect5, vect6, vect7};

int correspondances_forme_position[] = {-1, 0, 0, 0,
                                        0, 0, 0}; // on ne met que le y
string correspondance_couleurs[] = {"Black", "Red", "Green", "Blue",
                                    "Yellow", "Magenta", "Cyan"};

Piece::Piece(enum type typeP, enum color colorP, int x_premiere_case,
             int y_premiere_case)
{
  piece_type = typeP;
  piece_color = colorP;
  pos_premiere_case = {x_premiere_case, y_premiere_case};
  matrice = correspondances_forme_matrice[typeP]; // petite matrice 3*3, 2*2 ou
                                                  // 4*4 contenant la forme// 1
                                                  // = occup�e, 0= non occup�e
  taille_matrice = int(sqrt(matrice.size()));     // taille de la matrice
  rotation = 0;                                   // appartient � |[0, 3]| la rotation r�elle en sera un multiple:
                                                  // aide � rep�rer la forme dans le plan
  std::cout << "Piece created" << std::endl;
}

Piece::Piece()
{
  Piece(I, Red, 0, correspondances_forme_position[I]); // temporel// a changer
  std::cout << "Piece created" << std::endl;
}

Piece::~Piece() { std::cout << "Piece deleted" << std::endl; }

void Piece::incorporate()
{
  int len = taille_matrice;
  for (int i = 0; i < len; i++)
  {
    for (int j = 0; j < len; j++)
    {
      monde->append_case(i + pos_premiere_case[0], j + pos_premiere_case[1],
                         piece_color);
    }
  }
  monde->accept();
}
bool Piece::move(enum direction dir)
{
  int n = monde->WIDTH;
  int m = monde->HEIGHT;
  bool permission = true; // = feasable
  int len = taille_matrice;
  int X, Y; // de combien on bouge dans chaque direction
  auto matrice_temp = matrice;
  switch (dir)
  {
  case Up:
    X = 0;
    Y = -1;
    break;
  case Down:
    X = 0;
    Y = 1;
    break;
  case Left:
    X = -1;
    Y = 0;
    break;
  case Right:
    X = 1;
    Y = 0;
    break;
  }
  pos_premiere_case[0] += X;
  pos_premiere_case[1] += Y;
  for (int i = 0; i < len; i++)
  {
    for (int j = 0; j < len; j++)
    {
      if ((matrice_temp[len * j + i] == 1) &&
          (monde->get_case((i + pos_premiere_case[0]),
                           (j + pos_premiere_case[1])) != Black ||
           i + pos_premiere_case[0] >= n || i + pos_premiere_case[0] < 0 ||
           j + pos_premiere_case[1] >= m))
      {
        pos_premiere_case[0] -= X;
        pos_premiere_case[1] -=
            Y; // on retablit la position initiale si on n'a pas boug�
        if (dir == Down)
        {
          monde->change_piece();
        }
        return false;
      }
    }
  }
  matrice = matrice_temp;
  return permission;
}

void Piece::rotate(int quart_tour)
{
  bool continuation = true;
  if (quart_tour >= 0)
  {
    while (quart_tour > 0 && continuation)
    {
      continuation = elementary_rotate();
      quart_tour--;
    }
  }
  else
  {
    while (quart_tour < 0 && continuation)
    {
      continuation = elementary_unrotate();
      quart_tour++;
    }
  }
}

bool Piece::elementary_unrotate()
{
  int n = monde->WIDTH;
  bool permission = true; // = feasable
  int len = taille_matrice;
  auto matrice_temp = matrice;
  for (int i = 0; i < taille_matrice; i++)
  {
    for (int j = 0; j < taille_matrice; j++)
    {
      // matrice_temp [taille_matrice*j+i] =
      // matrice[taille_matrice*(taille_matrice-1-i) + j];
      matrice_temp[taille_matrice * i + j] =
          matrice[taille_matrice * (taille_matrice - 1 - j) + i];
      //std::cout << "case " << i << ", " << j << "; " << pos_premiere_case[0] + j
      //         << "; " << matrice_temp[i + taille_matrice * j] << std::endl;
      if (pos_premiere_case[0] + j < n &&
          pos_premiere_case[0] + j >=
              0) // si la case appartient � la grande matrice
      {
        if (monde->get_matrix()[taille_matrice * (pos_premiere_case[0] + i) +
                                (pos_premiere_case[1] + j)] != Black &&
            matrice_temp[taille_matrice * i + j] == 1)
        {
          return false;
        }
      }
      else // si elle n'y appartient pas... il faut que la case de la petite
           // matrice ne soit pas occup�e
      {
        if (matrice_temp[taille_matrice * i + j] == 1)
        {
          return false;
        }
      }
    }
  }
  matrice = matrice_temp;
  rotation++;
  return permission;
  // NB::ici on utilise partout [taille*i+j]; pas une faute; rester coherent av
  // la formule de rotation
}

bool Piece::elementary_rotate()
{
  int n = monde->WIDTH;
  bool permission = true; // = feasable
  int len = taille_matrice;
  auto matrice_temp = matrice;
  for (int i = 0; i < taille_matrice; i++)
  {
    for (int j = 0; j < taille_matrice; j++)
    {
      // matrice_temp[taille_matrice*(taille_matrice-1-j) + i] =
      // matrice[taille_matrice*i+j] ;
      matrice_temp[taille_matrice * (taille_matrice - 1 - j) + i] =
          matrice[taille_matrice * i + j];
      //std::cout << "case " << i << ", " << j << "; " << pos_premiere_case[0] + j
      //          << std::endl;
      if (pos_premiere_case[0] + j < n &&
          pos_premiere_case[0] + j >= 0) // si la case appartient � la matrice
      {
        if (monde->get_matrix()[(pos_premiere_case[0] + i) +
                                taille_matrice * (pos_premiere_case[1] + j)] !=
                Black &&
            matrice_temp[i + taille_matrice * j] == 1)
        {
          return false;
        }
      }
      else // si elle n'y appartient pas... il faut que la case de la petite
           // matrice ne soit pas occup�e
      {
        if (matrice_temp[i + taille_matrice * j] == 1)
        {
          return false;
        }
      }
    }
  }
  matrice = matrice_temp;
  rotation--;
  return permission;
}

int Piece::get_case(int i, int j) { return matrice[i + j * taille_matrice]; }

Board::Board(int pas, int hauteur, int largeur, enum type pieceT,
             int numero_joueur)
{
  nr_joueur = numero_joueur;
  STEP = pas;
  HEIGHT = hauteur;
  WIDTH = largeur;
  int taille_matrice = largeur * hauteur;
  enum color *matrice = new enum color[taille_matrice];
  for (int i = 0; i < largeur; i++)
  {
    for (int j = 0; j < hauteur; j++)
      matrice[j * WIDTH + i] = Black;
  }
  MATRIX = matrice;

  Piece *piece =
      new Piece(pieceT, Red, 0, correspondances_forme_position[pieceT]);
  current_piece = piece;
  piece->monde = this;
  std::cout << "Matrice de jeu initialisee" << std::endl;
}

Board::~Board()
{
  delete MATRIX;
  delete current_piece; // celle-ci a ete instanci�e avec Board::nouvelle_piece
                        // ()
  std::cout << "Board deleted succesflully" << std::endl;
}

enum color Board::get_case(int i, int j) { return MATRIX[WIDTH * j + i]; }

void Board::append_case(int i, int j, enum color couleur)
{
  MATRIX[WIDTH * j + i] = couleur;
}

void Board::nouvelle_piece() // pour l'instant, on n'ajoute que des cubes
{
  enum type pieceT = *pieces_suivantes.begin();
  Piece *piece = new Piece(pieceT, Red, int(WIDTH / 2) - 2,
                           correspondances_forme_position[pieceT]);
  pieces_suivantes.pop_front();
  current_piece = piece;
  piece->monde = this;
}

void Board::accept()
{
  int x = current_piece->pos_premiere_case[0];
  int y = current_piece->pos_premiere_case[1];
  for (int i = 0; i < current_piece->taille_matrice; i++)
  {
    for (int j = 0; j < current_piece->taille_matrice; j++)
    {
      if (current_piece->get_case(i, j) == 1)
        append_case(x + i, y + j, current_piece->piece_color);
    }
  }
}

enum color *Board::get_matrix() { return (MATRIX); }

void Board::continuer()
{
  int y_avant = current_piece->pos_premiere_case[1];
  enum type Tavant = current_piece->piece_type;
  bool est_tombee = current_piece->move(Down);
  if (est_tombee == false &&
      y_avant == correspondances_forme_position[Tavant])
  {
    board_over = true;
    Partie->stop_game(nr_joueur);
    std::cout << "joueur " << nr_joueur
              << " a fini premier ! : " << current_piece->pos_premiere_case[0]
              << std::endl;
  }
  // cout<< "Etape"<<  std::endl;
}

void Board::change_piece()
{
  int x = current_piece->pos_premiere_case[0];
  int y = current_piece->pos_premiere_case[1];
  int n = current_piece->taille_matrice;
  accept();
  delete current_piece;
  check_lines(
      x, y,
      n); // on suppose que la suppression de ligne ne peut se produire qu'ici
  nouvelle_piece();
  Partie->broadcast_piece(); // on demande a la partie de prevoir une piece
                             // supplementaire
  std::cout << "piece changed state" << std::endl;
}

void Board::append_piece(enum type pieceT)
{
  pieces_suivantes.push_back(pieceT);
}

void Board::executer_commandes()
{
  bool a_bouge;
  while (commandes_recues.begin() != commandes_recues.end())
  {
    switch (*commandes_recues.begin())
    {
    case move_right:
      a_bouge = current_piece->move(Right);
      std::cout << "piece is moving right" << a_bouge << std::endl;
      break;
    case move_left:
      a_bouge = current_piece->move(Left);
      std::cout << "piece is moving left" << a_bouge << std::endl;
      break;
    case move_down:
      a_bouge = current_piece->move(Down);
      std::cout << "piece is moving down" << a_bouge << std::endl;
      break;
    case rotate_direct:
      a_bouge = current_piece->elementary_rotate();
      std::cout << "piece is rotating left" << a_bouge << std::endl;
      break;
    case rotate_undirect:
      a_bouge = current_piece->elementary_unrotate();
      std::cout << "piece is rotating right" << a_bouge << std::endl;
      break;
    }

    commandes_recues.pop_front();
  }
}

void Board::check_lines(
    int x, int y, int n) // ici i est la ligne  et j la colonne:: attention !!
{
  for (int i = y; i < y + n; i++)
  {
    bool bouge = true; // on supprime la ligne
    for (int j = 0; j < WIDTH; j++)
    {
      if (MATRIX[WIDTH * i + j] == Black)
        bouge = false;
    }
    if (bouge == true)
    {
      for (int k = i; k > 0; k--)
      {
        for (int j = 0; j < WIDTH; j++)
        {
          MATRIX[WIDTH * k + j] = MATRIX[WIDTH * (k - 1) + j];
        }
      }
      for (int j = 0; j < WIDTH; j++)
      {
        MATRIX[j] = Black;
      }
    }
  }
}

////////Game ///////
Game::Game(int width, int height, int step, int nb_joueurs)
{
  WIDTH = width;
  HEIGHT = height;
  STEP = step;
  joueurs = nb_joueurs;
  game_over = false;
  for (int i = 0; i < joueurs; i++)
  {
    nouvelle_piece();
    Board *monde = new Board(STEP, HEIGHT, WIDTH, *pieces_suivantes.begin(), i);
    Jeux.push_back(monde);
    monde->Partie = this;
  }
  pieces_suivantes
      .pop_front(); // on a deja donn� cette piece lors de l'initialisation; on
                    // la retire et on la remplace par une autre;
  nouvelle_piece();
  broadcast_piece(); // chaque Board en a besoin d'au moins
}

Game::~Game()
{
  for (int i = 0; i < joueurs; i++)
    delete Jeux[i];

  std::cout << "Game data removed successfully" << std::endl;
}

void Game::nouvelle_piece()
{
  int nrPiece = std::rand() % 7;
  pieces_suivantes.push_back((enum type)nrPiece);
}

void Game::broadcast_piece()
{
  for (int i = 0; i < joueurs; i++)
  {
    Jeux[i]->append_piece(*pieces_suivantes.begin());
  }
  pieces_suivantes.pop_front();
  nouvelle_piece();
}

void Game::continuer()
{
  for (int i = 0; i < joueurs; i++)
    Jeux[i]->continuer();
}

void Game::stop_game(int numero_joueur)
{
  game_over = true;
  std::cout << "Game Over !" << std::endl
            << "Player " << numero_joueur << " wins !" << std::endl;
}
