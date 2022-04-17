#pragma once
#include "../common/Vector.hpp"
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

  public:
    Player_jump_checker();
    void moveto (const Vector3 &pos);
    float getJumpDistance ();
    float getJumpDistance (float height);
    float jumpHeight ();
    bool isJumpPossible (Vector3 position);
    float distance (Vector3 &position);
};