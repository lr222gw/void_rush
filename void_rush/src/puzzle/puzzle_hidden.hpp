#pragma once

#include "puzzle.hpp"

class HiddenPuzzle : public Puzzle
{
private:

public:
    HiddenPuzzle(int seed, Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager);

    void randomizeValue(float &value, float &value2);

    void Interaction(vec3 playerPos, vec3 forwardVec) override;

    void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position, ExtraArgs extraArgs) override;

    void Update(Graphics*& gfx) override;
};