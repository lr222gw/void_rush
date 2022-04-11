#include "puzzle_hidden.hpp"

HiddenPuzzle::HiddenPuzzle(const Vector3& position, int seed) : Puzzle(position, seed)
{
    
}

void HiddenPuzzle::Interaction()
{
    //Check if left click is pressed while player is close enough and looking at the hitbox of the key.
}

void HiddenPuzzle::InitiatePuzzle()
{
    //Based on the size of the platform, place out the key in a valid position where the key can be reached by the player.
    srand(Puzzle::GetSeed());
    
}
