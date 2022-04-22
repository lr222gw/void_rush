#include "puzzle.hpp"

Puzzle::Puzzle(int seed, Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler)
    : seed(seed), Portal(gfx, rm, colHandler)
{
}

Puzzle::~Puzzle()
{
    for (size_t i = 0; i < puzzleObjects.size(); i++) {
        this->GetColHandler()->deletePlatform(puzzleObjects[i]);
        delete puzzleObjects[i];
    }
    puzzleObjects.clear();
    if (puzzlePlatform != nullptr)
    {
        this->GetColHandlerBase()->deletePlatform(puzzlePlatform);
        delete puzzlePlatform;
    }
}

bool Puzzle::GetState () const { return this->GetStatePortal(); }

void Puzzle::SetPosition(vec3 pos)
{
    this->position = pos;
}

void Puzzle::ResetState()
{
    for (size_t i = 0; i < puzzleObjects.size(); i++) {
        this->GetColHandler()->deletePlatform(puzzleObjects[i]);
        delete puzzleObjects[i];
    }
    puzzleObjects.clear();
    if (puzzlePlatform != nullptr)
    {
        this->GetColHandlerBase()->deletePlatform(puzzlePlatform);
        delete puzzlePlatform;
        puzzlePlatform = nullptr;
    }
    this->ResetPortal();
}

vec3 Puzzle::GetPosition() const
{
    return this->position;
}

int Puzzle::GetSeed() const
{
    return seed;
}

CollisionHandler* Puzzle::GetColHandler()
{
    return this->GetColHandlerBase();
}

void Puzzle::SpawnDoor (vec3 pos)
{
    // Spawn the door after puzzle is completed.
    this->Spawn(pos);
}