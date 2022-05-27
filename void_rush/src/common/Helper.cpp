#include "Helper.hpp"
#include <corecrt_math.h>

float deg_to_rad (float deg)
{
    float ret = deg * ((float)M_PI / 180.0f);
    return ret;
}

float get_distance(vec3 one, vec3 two){
    return powf((powf((two.x - one.x),2)+powf((two.y - one.y),2)+powf((two.z - one.z),2)),0.5f);
}

vec2 Projectile(float rad, float time, vec2 velocity)
{
    vec2 pos;
    pos.x = (float)(velocity.x*cos(rad)*time);
    pos.y = (float)((velocity.y*sin(rad))*time - (9.82f*pow(time,2))/2);
    return pos;
}

vec3 Projectile(float rad_x, float rad_z, float time, vec3 velocity)
{
    vec3 pos;
    pos.x = (float)(velocity.x*cos(rad_x)*time*cos(rad_z));
    pos.y = (float)((velocity.y*sin(rad_z))*time - (9.82f*pow(time,2))/2);
    pos.z = (float)(velocity.z*cos(rad_x)*time*sin(rad_z));
    return pos;
}

float dot (vec3 one, vec3 two)
{
    return (one.x * two.x) + (one.y * two.y) + (one.z * two.z);
}
float vec_len (vec3 vec)
{
    return sqrtf (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

vec3 normal_to_rot(vec3& from, const vec3& to)
{
    vec3 temp = to;
    vec3 normal = to;
    vec3 axis = from.X(to);
    float dot = from * temp;
    float magnitude = from.length() * temp.length();
    float angle = acosf(dot/magnitude);

    if (dot == -1) {
        axis = vec3(1, 0, 0);
    }

    axis.Normalize();
    axis = axis * angle;
    return axis;
}
