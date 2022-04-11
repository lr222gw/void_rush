#pragma once
#include "../common/Vector.hpp"
#include <math.h>
#include <vector>

class player
{
  private:
    Vector3 pos;
    float jumpvel;
    float speed;
    float gravity;
    float launchangle;

  public:
    player ();
    void moveto (const Vector3 &pos);
    float getJumpDistance ();
    float getJumpDistance (float height);
    float jumpHeight ();
    bool isJumpPossible (Vector3 position);
    float distance (Vector3 &position);
};