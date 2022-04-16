#pragma once
#include "Platform.hpp"
#include "playerGen.hpp"
#include <random>


class generation
{
  private:
    int seed;
    int elements;
    Platform*startPlat;
    std::vector<Platform*> anchors;
    player *pl;

  public:
    generation (int seed, int elements);
    ~generation ();
    bool start (int selectedDiff);
    std::vector<Platform*> getPlatforms ();
    void assignPlayer (player *player);
    float randF (float min, float max);
};