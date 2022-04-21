#pragma once

#include "puzzle.hpp"

#include "generation/Generation_manager.hpp"
#include "3Dproj/CollisionHandler.h"

class MathPuzzle : public Puzzle
{
private:
    int choices[3] = {0, 0, 0};
    int components[3] = {0, 0, 0};
    char arithmetic = '0';
    const int maxValuePlusMinus = 100;
    const int maxValueMultiplication = 10;
    const int maxValueDivision[2] = {91, 4};

    CollisionHandler* collHandl;
public:
    MathPuzzle(int seed,Graphics*& gfx, ResourceManager*& rm, Generation_manager*& generationManager, CollisionHandler* collHandl);

    std::string GetComponents() const;

    void Interaction(vec3 playerPos, vec3 forwardVec) override;

    void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position) override;

    void Update(Graphics*& gfx) override;
};