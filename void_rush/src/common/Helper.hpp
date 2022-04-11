#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector.hpp"

float deg_to_rad (float deg);

Vector2 Projectile(float rad, float time, Vector2 velocity);