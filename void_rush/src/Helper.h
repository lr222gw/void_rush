#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

float deg_to_rad(float deg){
    float ret = deg * (M_PI/180);
    return ret;
}