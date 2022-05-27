#pragma once

#include "puzzle.hpp"
#include "3Dproj/3DLettersHandler.h"

class MemoryPuzzle : public Puzzle
{
private:
    Letters3DHandler* letters;
    Letters3DHandler* answerLetters;
    int answer[3] = {0, 0, 0};
    int currentAnswer[3] = { -1, -1, -1 };
    int currentNumber = 0;
public:
    MemoryPuzzle(int seed, Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager);
    virtual ~MemoryPuzzle();

    void Interaction(vec3 playerPos, vec3 forwardVec) override;

    void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position, ExtraArgs extraArgs) override;

    void Update(Graphics*& gfx) override;
};