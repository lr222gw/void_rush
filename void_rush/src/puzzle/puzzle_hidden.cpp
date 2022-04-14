#include "puzzle_hidden.hpp"

HiddenPuzzle::HiddenPuzzle(const Vector3& position, int seed, int width, int length) : Puzzle(position, seed, width, length)
{
    
}

void HiddenPuzzle::Interaction(int choice)
{
    //Check if left click is pressed while player is close enough and looking at the hitbox of the key.
    std::cout << "Test" << std::endl;
}

void HiddenPuzzle::InitiatePuzzle()
{
    //Based on the size of the platform, place out the key in a valid position where the key can be reached by the player.
    srand(Puzzle::GetSeed());
    
}
