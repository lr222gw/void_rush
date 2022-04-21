#pragma once
#include "../common/Vector.hpp"
#include "3Dproj/Vec.h"
#include <math.h>
#include <vector>

class Player_jump_checker
{
  private:
    Vector3 pos;
    float jumpvel;
    float speed;
    float gravity;
    float launchangle;
    friend class ImguiManager;
  public:
    Player_jump_checker();
    void reset();
    void moveto (const vec3& pos);
    float getJumpDistance ();
    float getJumpDistance (float height);
    float jumpHeight ();
    bool isJumpPossible (vec3 position);
    float distance (vec3 &position);
};