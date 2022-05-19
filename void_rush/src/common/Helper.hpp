#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "3Dproj/Vec.h"

float deg_to_rad (float deg);
float get_distance(vec3 one, vec3 two);
float dot (vec3 one, vec3 two);
float vec_len (vec3 vec);
vec3 normal_to_rot(vec3& from, const vec3& to);

vec2 Projectile(float rad, float time, vec2 velocity);
vec3 Projectile(float rad_x, float rad_z, float time, vec3 velocity);