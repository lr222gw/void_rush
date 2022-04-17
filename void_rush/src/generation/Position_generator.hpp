#pragma once
#include "Platform.hpp"
#include "playerGen.hpp"
#include <random>


enum class Difficulity {
    easy
};

class Position_generator
{
  private:
    int seed;
    int elements;
    Platform*startPlat;
    std::vector<Platform*> anchors;
    Player_jump_checker* pl;

  public:
    Position_generator(int seed);
    Position_generator(int seed, int elements);
    ~Position_generator();
    bool start (Difficulity diff);
    std::vector<Platform*>* getPlatforms ();
    void assignPlayer (Player_jump_checker* player);
    float randF (float min, float max);
};