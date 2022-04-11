#pragma once
#include "../common/Vector.hpp"
#include <math.h>
#include <memory>
#include <vector>


class platform
{
  private:
    Vector3 pos;
    int obstacles;
    int difficluty;

  public:
    platform ();
    platform (std::vector<float> pos, int obstacles, int difficluty);
    platform (Vector3 pos, int obstacles, int difficluty);
    ~platform ();
    void setPosition (float xPos, float yPos, float zPos);
    void setPosition (Vector3 position);
    void move (float xOfset, float yOfset, float zOfset);
    Vector3 getPos ();
    float distance (Vector3 position) const;
    float distance (std::vector<float> &position) const;
    platform *next;
};