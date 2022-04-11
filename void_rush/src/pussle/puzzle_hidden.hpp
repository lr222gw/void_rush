#pragma once

#include "puzzle.hpp"

class HiddenPuzzle : public Puzzle
{
private:
    int width;
    int length;
    struct Key
    {
        //Load the key 3D object 
        Vector3 originPosition;


    };
public:
    HiddenPuzzle(const Vector3& position, int seed);

    void Interaction();

    void InitiatePuzzle() override;
};