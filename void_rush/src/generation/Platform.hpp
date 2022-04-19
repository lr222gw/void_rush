#pragma once
#include "../common/Matrix.hpp"
#include "../common/Shape.hpp"
#include "../common/Vector.hpp"
#include "../common/Shape.hpp"
#include "../common/Matrix.hpp"
#include <math.h>
#include <memory>
#include <vector>

class Platform 
{
  private:
    Vector3 pos;
    int obstacles;
    int difficluty;
    float rotation;
    Shape platformShape;
    RotationMatrix rotMat;
    friend class ImguiManager;
  public:
    Platform();
    Platform(std::vector<float> pos, int obstacles, int difficluty, float rotation = 0.0);
    Platform(Vector3 pos, int obstacles, int difficluty, float rotation = 0.0);
    ~Platform();
    void setPosition (float xPos, float yPos, float zPos);
    void setPosition (Vector3 position);
    void move (float xOfset, float yOfset, float zOfset);
    Vector3* getPos ();
    float getRotation () const;
    float distance (Vector3* position) const;
    float distance (std::vector<float> &position) const;
    Platform* next;
};