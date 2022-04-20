#pragma once
#include "Vector.hpp"
#include "3Dproj/Vec.h"
#include <math.h>


struct RotationMatrix
{
    float matrix[3][3];
    RotationMatrix ()
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                matrix[i][j] = 0.0f;
            }
        }
    }
    void rotateAroundX (double angle)
    {
        matrix[0][0] = 1;
        matrix[1][1] = cos (angle);
        matrix[1][2] = -sin (angle);
        matrix[2][1] = sin (angle);
        matrix[2][2] = cos (angle);
    }
    void rotateAroundY (double angle)
    {
        matrix[1][1] = 1;
        matrix[0][0] = cos (angle);
        matrix[0][2] = -sin (angle);
        matrix[2][0] = sin (angle);
        matrix[2][2] = cos (angle);
    }
    void rotateAroundZ (double angle)
    {
        matrix[2][2] = 1;
        matrix[0][0] = cos (angle);
        matrix[1][0] = -sin (angle);
        matrix[0][1] = sin (angle);
        matrix[1][1] = cos (angle);
    }
    void MultiplyVector (vec3 &vector)
    {
        vector.x = vector.x * matrix[0][0] + vector.x * matrix[0][1]
                   + vector.x * matrix[0][2];
        vector.y = vector.y * matrix[0][0] + vector.y * matrix[0][1]
                   + vector.y * matrix[0][2];
        vector.z = vector.z * matrix[0][0] + vector.z * matrix[0][1]
                   + vector.z * matrix[0][2];
    }
};