#pragma once

#include "puzzle.hpp"

class MemoryPuzzle : public Puzzle
{
private:

public:
    MemoryPuzzle(int seed, Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager);

    void Interaction(vec3 playerPos, vec3 forwardVec) override;

    void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position) override;

    void Update(Graphics*& gfx) override;
};