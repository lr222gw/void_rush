#pragma once
#include "../common/Matrix.hpp"
#include "../common/Shape.hpp"
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
    double rotation;
    Shape platformShape;
    RotationMatrix rotMat;

  public:
    platform ();
    platform (std::vector<float> pos, int obstacles, int difficluty, double rotation = 0.0);
    platform (Vector3 pos, int obstacles, int difficluty, double rotation = 0.0);
    ~platform ();
    void setPosition (float xPos, float yPos, float zPos);
    void setPosition (Vector3 position);
    void move (float xOfset, float yOfset, float zOfset);
    Vector3 getPos ();
    double getRotation () const;
    float distance (Vector3 position) const;
    float distance (std::vector<float> &position) const;
    platform *next;
};