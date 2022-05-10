#pragma once

#include "../common/Helper.hpp"
#include "../common/Vector.hpp"
#include "../physics/Jump.hpp"
#include <fstream>
#include "puzzle_math.hpp"
#include "puzzle.hpp"
#include "puzzle_hidden.hpp"
#include "puzzle_memory.hpp"
#include "3Dproj/SoundManager.h"
#include <vector>

class ProtoPuzzle
{
private:
	std::vector<Puzzle*> puzzleList;
	int chosenPuzzle = -1;
	int seed;
	const int PUZZLES = 3; //Set to amount of puzzles done.
	MathPuzzle* math;
	HiddenPuzzle* hidden;
	MemoryPuzzle* memory;
	Graphics*& gfxPuzzle;
	ResourceManager*& rmPuzzle;

public:
	ProtoPuzzle(Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager);
	~ProtoPuzzle();
	void Initiate(vec3 platformPosition);
	int ChoosePuzzle();
	void Interact(vec3 playerPos, vec3 forwardVec);
	void UpdatePlayerPosition(vec3 Pos);
	void Update();
	bool isCompleted();
};