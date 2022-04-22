#pragma once

#include "puzzle.hpp"

class HiddenPuzzle : public Puzzle
{
private:

public:
    HiddenPuzzle(int seed, Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler);

    void Interaction(vec3 playerPos, vec3 forwardVec) override;

    void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position) override;

    void Update(Graphics*& gfx) override;
};