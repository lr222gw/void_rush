#include "puzzle.hpp"

Puzzle::Puzzle (const Vector3 &position, int seed, int width, int length, bool completed)
    : seed (seed), position (position), width(width), length(length), completed (completed)
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

bool Puzzle::GetState () const { return completed; }

void Puzzle::ResetState()
{
    completed = false;
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
    completed = true;
    // Spawn the door after puzzle is completed.
}