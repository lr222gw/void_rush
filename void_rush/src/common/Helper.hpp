#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector.hpp"
#include "3Dproj/Vec.h"

float deg_to_rad (float deg);
float get_distance(Vector3 one, Vector3 two);
float dot (Vector3 one, Vector3 two);
float vec_len (Vector3 vec);
vec3 normal_to_rot(vec3& from, const vec3& to);

Vector2 Projectile(float rad, float time, Vector2 velocity);
Vector3 Projectile(float rad_x, float rad_z, float time, Vector3 velocity);