#pragma once
#include "../common/Matrix.hpp"
#include "../common/Shape.hpp"
#include "../common/Vector.hpp"
#include "../common/Shape.hpp"
#include "../common/Matrix.hpp"
#include "3Dproj/Vec.h"
#include <math.h>
#include <memory>
#include <vector>

class Platform 
{
  private:
    vec3 pos;
    int obstacles;
    int difficluty;
    float rotation;
    Shape platformShape;
    RotationMatrix rotMat;
    friend class ImguiManager;
  public:
    Platform();
    Platform(std::vector<float> pos, int obstacles, int difficluty, double rotation = 0.0);
    Platform(vec3 pos, int obstacles, int difficluty, double rotation = 0.0);
    ~Platform();
    void setPosition (float xPos, float yPos, float zPos);
    void setPosition (vec3 position);
    void move (float xOfset, float yOfset, float zOfset);
    vec3* getPos ();
    float getRotation () const;
    float distance (vec3* position) const;
    float distance (std::vector<float> &position) const;
    Platform* next;
};