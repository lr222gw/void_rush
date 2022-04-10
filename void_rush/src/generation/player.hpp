#pragma once
#include "../common/Vector.hpp"
#include <math.h>
#include <vector>

class player
{
  private:
    float jumpvel;
    float speed;
    float launchangle;
    float gravity;
    Vector3 pos;

  public:
    player ();
    void moveto (const Vector3 &pos);
    float getJumpDistance ();
    float getJumpDistance (float height);
    float jumpHeight ();
    bool isJumpPossible (Vector3 position);
    float distance (Vector3 &position);
};