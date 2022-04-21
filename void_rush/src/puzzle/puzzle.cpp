#include "puzzle.hpp"

Puzzle::Puzzle(int seed, Graphics*& gfx, ResourceManager*& rm)
    : seed(seed), Portal(gfx, rm)
{
}

Puzzle::~Puzzle()
{
    for (size_t i = 0; i < puzzleObjects.size(); i++) {
        delete puzzleObjects[i];
    }
    puzzleObjects.clear();
    if (puzzlePlatform != nullptr)
    {
        delete puzzlePlatform;
    }
}

bool Puzzle::GetState () const { return this->GetStatePortal(); }

void Puzzle::ResetState()
{
    for (size_t i = 0; i < puzzleObjects.size(); i++) {
        delete puzzleObjects[i];
    }
    puzzleObjects.clear();
    if (puzzlePlatform != nullptr)
    {
        delete puzzlePlatform;
    }
    this->ResetPortal();
}

int Puzzle::GetSeed() const
{
    return seed;
}

void Puzzle::SpawnDoor ()
{
    // Spawn the door after puzzle is completed.
    this->Spawn();
}