#include "Helper.hpp"
#include <corecrt_math.h>

float deg_to_rad (float deg)
{
    float ret = deg * ((float)M_PI / 180.0f);
    return ret;
}

Vector2 Projectile(float rad, float time, Vector2 velocity)
{
    Vector2 pos;
    pos.x = velocity.x*cos(rad)*time;
    pos.y = (velocity.y*sin(rad))*time - (9.82*pow(time,2))/2;
    return pos;
}

Vector3 Projectile(float rad_x, float rad_z, float time, Vector3 velocity)
{
    Vector3 pos;
    pos.x = velocity.x*cos(rad_x)*time*cos(rad_z);
    pos.y = (velocity.y*sin(rad_z))*time - (9.82*pow(time,2))/2;
    pos.z = velocity.z*cos(rad_x)*time*sin(rad_z);
    return pos;
}

