#pragma once

#include "puzzle.hpp"

class HiddenPuzzle : public Puzzle
{
private:
    struct Key
    {
        //Load the key 3D object 
        Vector3 originPosition;


    };
public:
    HiddenPuzzle(const Vector3& position, int seed, int width, int length);

    void Interaction(vec3 playerPos) override;

    void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm) override;

    void Update(Graphics*& gfx) override;
};