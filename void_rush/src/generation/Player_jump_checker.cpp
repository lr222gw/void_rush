#include "Player_jump_checker.hpp"

Player_jump_checker::Player_jump_checker()
    : pos ({ 0.0f, 0.0f, 0.0f }), jumpvel (5.0f), speed (5.0f),
      gravity (9.8f), launchangle (45.0f)
{
}

void Player_jump_checker::reset()
{
    this->pos.x = 0.f;
    this->pos.y = 0.f;
    this->pos.z = 0.f;
    
}

void Player_jump_checker::set_physics_params(float jumpvel, float speed, float gravity)
{

    this->jumpvel = jumpvel;
    this->speed = speed;
    this->gravity = -gravity;

    this->launchangle = 45.f; //NOTE: might want to calculate this later.

}

void Player_jump_checker::moveto (const vec3&pos)
{
    this->pos.x = pos.x;
    this->pos.y = pos.y;
    this->pos.z = pos.z;
}
vec3 Player_jump_checker::getPos() const
{
    return this->pos;
}
float Player_jump_checker::getJumpDistance ()
{
    float angle = acos((speed * 1.0f + this->jumpvel * 0.0) /
        (sqrtf(powf(this->speed, 2) + powf(this->jumpvel, 2))));

    float vel = sqrtf (powf (this->speed, 2.0) + powf (this->jumpvel, 2.0f));
    float time = (float)((vel * sinf (angle)
               + sqrtf (std::abs(powf (vel * sinf (angle), 2.0f)
                   + 2 * this->gravity * this->pos.y)))
              / this->gravity);

    return this->speed * time;
}

float Player_jump_checker::getJumpDistance (float height)
{

    float angle = acos((speed * 1.0f + this->jumpvel * 0.0 ) /
        (sqrtf(powf(this->speed, 2) + powf(this->jumpvel, 2))));
    

    float vel = sqrtf (powf (this->speed, 2.0) + powf (this->jumpvel, 2.0f));
    float time = (float)((vel * sinf (angle)
            + sqrtf (std::abs(powf (vel * sinf (angle), 2.0f)
            + 2 * this->gravity * (this->pos.y - height))))
              / this->gravity);

    return this->speed * time;
}

float Player_jump_checker::jumpHeight ()
{
    float angle = acos((speed * 1.0f + this->jumpvel * 0.0) /
        (sqrtf(powf(this->speed, 2) + powf(this->jumpvel, 2))));

    //float vel = sqrtf (powf (this->speed, 2.0) + powf (this->jumpvel, 2.0f));
    float vel = this->jumpvel;
    return this->pos.y
           + (powf (vel, 2.0f) * powf (sinf (angle), 2.0f)
              / (2 * this->gravity));
}

bool Player_jump_checker::isJumpPossible (vec3 position)
{
    float jumpheight = jumpHeight ();
    float heightDif = jumpheight - position.y;
    float jumpDist = getJumpDistance (position.y);
    float distanceDif = jumpDist - this->distance (position);

    if (heightDif <= 0)
    {
        return false;
    }
    if (distanceDif <= 0 && jumpDist != 0.f)
    {
        return false;
    }
    return true;
}

float Player_jump_checker::distance (vec3 &position)
{
    return sqrtf (powf (this->pos.x - position.x, 2.0)
                  + powf (this->pos.z - position.z, 2.0));
}
