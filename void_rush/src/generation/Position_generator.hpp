#pragma once
#include "Platform.hpp"
#include "playerGen.hpp"
#include "3Dproj/Vec.h"
#include <random>


enum class Difficulity {
    easy = 1, medium = 2, hard = 3
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
    void generate_anchor_positions(int platforms_between_anchors, Difficulity selectedDiff);
    void generate_jumpPoints_positions(Difficulity selectedDiff);
    void reset_generation(vec3 player_position);
    void set_seed(int _seed);
    std::vector<Platform*>* getAnchors ();
    std::vector<Platform*>* getJumpPoints ();
    void setNrOfElements(int nrOfElements);
    void assignPlayer (Player_jump_checker* player);
    float randF (float min, float max);
};