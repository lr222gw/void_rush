#pragma once

#include "../common/Helper.hpp"
#include "../common/Vector.hpp"
#include "../physics/Jump.hpp"
#include <fstream>
#include "puzzle_math.hpp"
#include "puzzle.hpp"
#include "puzzle_hidden.hpp"
#include <vector>

class ProtoPuzzle
{
private:
	std::vector<Puzzle*> puzzleList;
	int chosenPuzzle;
	int seed;
	const int PUZZLES = 1; //Set to amount of puzzles done.
	MathPuzzle* math;
	HiddenPuzzle* hidden;

public:
	ProtoPuzzle();
	~ProtoPuzzle();
	int ChoosePuzzle();
	void Interact(int choice);
};