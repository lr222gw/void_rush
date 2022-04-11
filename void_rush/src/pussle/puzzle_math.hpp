#pragma once

#include "puzzle.hpp"

class MathPuzzle : public Puzzle
{
private:
    int choices[3];
    int components[3];
    char arithmetic;
    const int maxValuePlusMinus = 100;
    const int maxValueMultiplication = 10;
    const int maxValueDivision[2] = {91, 4};
public:
    MathPuzzle(const Vector3& position, int seed);

    std::string GetComponents() const;

    void Interaction();

    void InitiatePuzzle() override;
};