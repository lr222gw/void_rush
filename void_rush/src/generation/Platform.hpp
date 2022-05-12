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
    int obstacles; //NOTE: To use in future
    int difficluty;//NOTE: To use in future    
    friend class ImguiManager;

public:

    Shape platformShape;
    Platform();
    Platform(vec3 pos, int obstacles = 0, int difficluty = 1);
    ~Platform();
    void setPosition(float xPos, float yPos, float zPos);
    void setPosition(vec3 position);
    void move(float xOfset, float yOfset, float zOfset);
    vec3* getPos();
    float distance(vec3* position) const;
    float distance(std::vector<float>& position) const;
    Platform* next;
    Platform* prev;
};
    