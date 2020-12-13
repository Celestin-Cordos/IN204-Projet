#ifndef GAMECLASS_HPP
#define GAMECLASS_HPP

#include <iostream>
#include <vector>
using namespace std;

enum color {Nan, red, green, bleue, yellow, pink, purple};
enum type {I, O, T, L, J, Z, S};
enum direction {Left, Right};

class Case {
	bool filled;
	color case_color;
};

class Piece {
public:
	type piece_type;
	color piece_color;
	//list of positions (cases) it's filling in the matrix
	vector< tuple<int, int> > posi_arr;
	
	Piece();
	Piece(type piece_type);
	~Piece();

	void rotate(unsigned int deg);
	void move(direction dir);
};

class Game {
public:
	Case board[20][10];

	Game();
	~Game();
};


#endif