#include "puzzle.hpp"

Puzzle::Puzzle(const Vector3& position, int seed, int width, int length, Graphics*& gfx, ResourceManager*& rm, bool completed)
    : seed(seed), position(position), width(width), length(length), completed(completed), Portal(gfx, rm)
{
    //Create door position based on width and length of the platform together with the origin point. 
    //Example: if width is 500, then one of the corners is at half of width + origin coordinates.
}

Puzzle::~Puzzle()
{
    for (size_t i = 0; i < puzzleObjects.size(); i++) {
        delete puzzleObjects[i];
    }
}

bool Puzzle::GetState () const { return this->GetStatePortal(); }

void Puzzle::ResetState()
{
    this->ResetPortal();
}

int Puzzle::GetWidth() const
{
    return width;
}

int Puzzle::GetLength() const
{
    return length;
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