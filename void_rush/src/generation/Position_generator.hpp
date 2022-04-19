#pragma once
#include "Platform.hpp"
#include "playerGen.hpp"
#include <random>


enum class Difficulity {
    easy = 1
};

class Position_generator
{
  private:
    int seed;
    int elements;
    Platform*startPlat;
    std::vector<Platform*> anchors;
    std::vector<Platform*> jumpPoints;
    Player_jump_checker* pl;
    friend class ImguiManager;
  public:
    Position_generator(int seed);
    Position_generator(int seed, int elements);
    ~Position_generator();
    bool start (Difficulity diff);
    void reset_anchors();
    void set_seed(int _seed);
    std::vector<Platform*>* getPlatforms ();
    void setNrOfElements(int nrOfElements);
    void assignPlayer (Player_jump_checker* player);
    float randF (float min, float max);
};