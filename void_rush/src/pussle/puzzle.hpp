#pragma once

//Add includes here
#include "../common/Vector.hpp"
#include <random>
#include <string>

class Puzzle
{
private:

protected:
    int seed;
    Vector3 position;
    bool completed;

public:
    //Create puzzle
    Puzzle(const Vector3& position, int seed, bool completed = false);

    bool GetState() const;

    int GetSeed() const;

    void SpawnDoor();

    //Pick the correct type of puzzle and initiate it.
    virtual void InitiatePuzzle() = 0;
};