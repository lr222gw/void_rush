#pragma once

#include "puzzle.hpp"

class HiddenPuzzle : public Puzzle
{
private:

public:
    HiddenPuzzle(const Vector3& position, int seed, int width, int length, Graphics*& gfx, ResourceManager*& rm);

    void Interaction(vec3 playerPos, vec3 forwardVec) override;

    void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm) override;

    void Update(Graphics*& gfx) override;
};