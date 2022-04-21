#include "Helper.hpp"
#include <corecrt_math.h>

float deg_to_rad (float deg)
{
    float ret = deg * ((float)M_PI / 180.0f);
    return ret;
}

float get_distance(Vector3 one, Vector3 two){
    //d = ((x2 - x1)2 + (y2 - y1)2 + (z2 - z1)2)1/2
    return powf((powf((two.x - one.x),2)+powf((two.y - one.y),2)+powf((two.z - one.z),2)),0.5f);
}

Vector2 Projectile(float rad, float time, Vector2 velocity)
{
    Vector2 pos;
    pos.x = velocity.x*cos(rad)*time;
    pos.y = (velocity.y*sin(rad))*time - (9.82f*pow(time,2))/2;
    return pos;
}

Vector3 Projectile(float rad_x, float rad_z, float time, Vector3 velocity)
{
    Vector3 pos;
    pos.x = velocity.x*cos(rad_x)*time*cos(rad_z);
    pos.y = (velocity.y*sin(rad_z))*time - (9.82f*pow(time,2))/2;
    pos.z = velocity.z*cos(rad_x)*time*sin(rad_z);
    return pos;
}

float dot (Vector3 one, Vector3 two)
{
    return (one.x * two.x) + (one.y * two.y) + (one.z * two.z);
}
float vec_len (Vector3 vec)
{
    return sqrtf (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}