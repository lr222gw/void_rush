#include "playerGen.hpp"

Player_jump_checker::Player_jump_checker()
    : pos ({ 0.0f, 0.0f, 0.0f }), jumpvel (20.0f), speed (5.0f),
      gravity (9.8f), launchangle (45.0f)
{
}

void Player_jump_checker::moveto (const Vector3 &pos)
{
    this->pos.x = pos.x;
    this->pos.y = pos.y;
    this->pos.z = pos.z;
}
float Player_jump_checker::getJumpDistance ()
{
    float vel = sqrtf (powf (this->speed, 2.0) + powf (this->jumpvel, 2.0f));
    float time = (vel * sin (this->launchangle)
               + sqrtf (powf (vel * sinf (this->launchangle), 2.0f)
                        + 2 * this->gravity * this->pos.z))
              / this->gravity;

    return this->speed * time;
}

float Player_jump_checker::getJumpDistance (float height)
{
    float vel = sqrtf (powf (this->speed, 2.0) + powf (this->jumpvel, 2.0f));
    float time = (vel * sin (this->launchangle)
               + sqrtf (powf (vel * sinf (this->launchangle), 2.0f)
                        + 2 * this->gravity * (this->pos.z - height)))
              / this->gravity;

    return this->speed * time;
}

float Player_jump_checker::jumpHeight ()
{
    float vel = sqrtf (powf (this->speed, 2.0) + powf (this->jumpvel, 2.0f));
    return this->pos.z
           + (powf (vel, 2.0f) * powf (sin (this->launchangle), 2.0f)
              / (2 * this->gravity));
}

bool Player_jump_checker::isJumpPossible (Vector3 position)
{
    float jumpheight = jumpHeight ();
    float heightDif = jumpheight - position.z;
    float jumpDist = getJumpDistance (position.z);
    float distanceDif = jumpDist - this->distance (position);
    /*if (heightDif <= 0) {
        plat->move(0, 0, heightDif);
    }
    if (distanceDif <= 0)
    {
        plat->move();
    }*/
    if (heightDif <= 0)
    {
        return false;
    }
    if (distanceDif <= 0)
    {
        return false;
    }
    return true;
}

float Player_jump_checker::distance (Vector3 &position)
{
    return sqrtf (pow (this->pos.x - position.x, 2.0)
                  + pow (this->pos.y - position.y, 2.0));
}
