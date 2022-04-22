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
	int chosenPuzzle = -1;
	int seed;
	const int PUZZLES = 2; //Set to amount of puzzles done.
	MathPuzzle* math;
	HiddenPuzzle* hidden;
	Graphics*& gfxPuzzle;
	ResourceManager*& rmPuzzle;

public:
	ProtoPuzzle(Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler);
	~ProtoPuzzle();
	void Initiate(vec3 platformPosition);
	int ChoosePuzzle();
	void Interact(vec3 playerPos, vec3 forwardVec);
	void Update();
};