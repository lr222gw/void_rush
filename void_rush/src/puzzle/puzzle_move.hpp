#pragma once

#include "puzzle.hpp"

class MovePuzzle : public Puzzle
{
private:
    float speed = 3.0f;
    vec3 startPos;
    vec2 speedDir = vec2(0.0f, 0.0f);
public:
    MovePuzzle(int seed, Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager);

    void Interaction(vec3 playerPos, vec3 forwardVec) override;

    void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position) override;

    void Update(Graphics*& gfx) override;
};