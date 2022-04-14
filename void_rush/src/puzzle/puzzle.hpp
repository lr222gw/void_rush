#pragma once

//Add includes here
#include "../common/Vector.hpp"
#include <random>
#include <string>
#include <iostream>
#include "3Dproj/GameObject.h"

class Puzzle
{
private:

protected:
    int seed;
    Vector3 position;
    int width;
    int length;
    Vector3 doorPosition;
    bool completed;

public:
    //Create puzzle
    Puzzle(const Vector3& position, int seed, int width, int length, bool completed = false);

    bool GetState() const;

    void ResetState();

    int GetWidth() const;
    
    int GetLength() const;

    int GetSeed() const;

    void SpawnDoor();

    virtual void Interaction(int choice) = 0;

    //Pick the correct type of puzzle and initiate it.
    virtual void InitiatePuzzle() = 0;
};