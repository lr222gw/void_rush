#pragma once
#include "Vector.hpp"
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
    void rotateAroundX (float angle)
    {
        matrix[0][0] = 1;
        matrix[1][1] = cosf (angle);
        matrix[1][2] = -sinf (angle);
        matrix[2][1] = sinf (angle);
        matrix[2][2] = cosf (angle);
    }
    void rotateAroundY (float angle)
    {
        matrix[1][1] = 1;
        matrix[0][0] = cosf (angle);
        matrix[0][2] = -sinf (angle);
        matrix[2][0] = sinf (angle);
        matrix[2][2] = cosf (angle);
    }
    void rotateAroundZ (float angle)
    {
        matrix[2][2] = 1;
        matrix[0][0] = cosf (angle);
        matrix[1][0] = -sinf (angle);
        matrix[0][1] = sinf (angle);
        matrix[1][1] = cosf (angle);
    }
    void MultiplyVector (Vector3 &vector)
    {
        vector.x = vector.x * matrix[0][0] + vector.x * matrix[0][1]
                   + vector.x * matrix[0][2];
        vector.y = vector.y * matrix[0][0] + vector.y * matrix[0][1]
                   + vector.y * matrix[0][2];
        vector.z = vector.z * matrix[0][0] + vector.z * matrix[0][1]
                   + vector.z * matrix[0][2];
    }
};