#pragma once
#include "Vector.hpp"
#include "3Dproj/Vec.h"
#include <vector>

struct inCorner {
    vec3 pos;
};

struct outCorner {
    vec3 pos;
};

struct line
{
    vec3 point1;
    vec3 point2;
};

struct Shape
{
    std::vector<vec3> Vertexes;
    inCorner inCorner;
    outCorner outCorner;

    Shape (){};
    Shape (std::vector<vec3> Vertexes)
    {
        Vertexes.assign (Vertexes.begin (), Vertexes.end ());
    }

    void addVertex (vec3 point) { Vertexes.push_back (point); }

    void setShapeCube (vec3 &center)
    {
        addVertex (center + vec3(-1, 1, 0));
        addVertex (center + vec3(1, 1, 0));
        addVertex (center + vec3(1, -1, 0));
        addVertex (center + vec3(-1, -1, 0));

        inCorner.pos = Vertexes[0];
        outCorner.pos = Vertexes[Vertexes.size() / 2];
    }
};