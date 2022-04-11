#include "puzzle.hpp"

Puzzle::Puzzle (const Vector3 &position, int seed, bool completed)
    : seed (seed), position (position), completed (completed)
{
    // EMPTY
}

bool Puzzle::GetState () const { return completed; }

int Puzzle::GetSeed() const
{
    return seed;
}

void Puzzle::SpawnDoor ()
{
    completed = true;
    // Spawn the door after puzzle is completed.
}