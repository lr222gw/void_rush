#pragma once

#include "puzzle.hpp"

#include "generation/Generation_manager.hpp"
#include "3Dproj/CollisionHandler.h"

class HiddenPuzzle : public Puzzle
{
private:
    CollisionHandler* collHandl;
public:
    HiddenPuzzle(int seed, Graphics*& gfx, ResourceManager*& rm, Generation_manager*& generationManager, CollisionHandler* collHandl);

    void Interaction(vec3 playerPos, vec3 forwardVec) override;

    void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position) override;

    void Update(Graphics*& gfx) override;
};