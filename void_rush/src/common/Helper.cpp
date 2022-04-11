#include "Helper.hpp"

float deg_to_rad (float deg)
{
    float ret = deg * (M_PI / 180);
    return ret;
}

Vector2 Projectile(float rad, float time, Vector2 velocity)
{
    Vector2 pos;
    pos.x = velocity.x*cos(rad)*time;
    pos.y = (velocity.y*sin(rad))*time - (9.82*pow(time,2))/2;
    return pos;
}

