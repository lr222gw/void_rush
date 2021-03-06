#pragma once

#include "puzzle.hpp"
#include "3Dproj/3DLettersHandler.h"

class MathPuzzle : public Puzzle
{
private:
    Letters3DHandler* letters;
    int choices[3] = {0, 0, 0};
    int components[3] = {0, 0, 0};
    char arithmetic = '0';
    const int maxValuePlusMinus = 100;
    const int maxValueMultiplication = 10;
    const int maxValueDivision[2] = {91, 4};
public:
    MathPuzzle(int seed,Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager);
    virtual ~MathPuzzle();

    std::string GetComponents() const;

    void Interaction(vec3 playerPos, vec3 forwardVec) override;

    void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position, ExtraArgs extraArgs) override;

    void Update(Graphics*& gfx) override;
};