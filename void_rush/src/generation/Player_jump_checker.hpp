#pragma once
#include "3Dproj/Vec.h"
#include <math.h>
#include <vector>

class Player_jump_checker
{
  private:
    vec3 pos;
    float jumpvel;
    float speed;
    float gravity;
    float launchangle;
    friend class ImguiManager;
  public:
    Player_jump_checker();
    void reset();
    void set_physics_params(float jumpvel, float speed, float gravity);
    void moveto (const vec3& pos);
    vec3 getPos() const;
    float getJumpDistance ();
    float getJumpDistance (float height);
    float jumpHeight ();
    bool isJumpPossible (vec3 position);
    float distance (vec3 &position);
};