#include "playerGen.hpp"

player::player ()
    : pos ({ 0.0f, 0.0f, 0.0f }), jumpvel (20.0f), speed (5.0f),
      gravity (9.8f), launchangle (45.0f)
{
}

void player::moveto (const Vector3 &pos)
{
    this->pos.x = pos.x;
    this->pos.y = pos.y;
    this->pos.z = pos.z;
}
float player::getJumpDistance ()
{
    float vel = sqrtf (powf (this->speed, 2.0) + powf (this->jumpvel, 2.0f));
    float t = (vel * sinf (this->launchangle)
               + sqrtf (powf (vel * sinf (this->launchangle), 2.0f)
                        + 2 * this->gravity * this->pos.z))
              / this->gravity;

    return this->speed * t;
}

float player::getJumpDistance (float height)
{
    float vel = sqrtf (powf (this->speed, 2.0) + powf (this->jumpvel, 2.0f));
    float t = (vel * sinf (this->launchangle)
               + sqrtf (powf (vel * sinf (this->launchangle), 2.0f)
                        + 2 * this->gravity * (this->pos.z - height)))
              / this->gravity;

    return this->speed * t;
}

float player::jumpHeight ()
{
    float vel = sqrtf (powf (this->speed, 2.0) + powf (this->jumpvel, 2.0f));
    return this->pos.z
           + (powf (vel, 2.0f) * powf (sinf (this->launchangle), 2.0f)
              / (2 * this->gravity));
}

bool player::isJumpPossible (Vector3 position)
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

float player::distance (Vector3 &position)
{
    return sqrtf (powf (this->pos.x - position.x, 2.0)
                  + powf (this->pos.y - position.y, 2.0));
}
