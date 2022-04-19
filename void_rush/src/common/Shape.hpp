#pragma once
#include "Vector.hpp"
#include <vector>

struct inCorner {
    Vector3 pos;
};

struct outCorner {
    Vector3 pos;
};

struct line
{
    Vector3 point1;
    Vector3 point2;
};

struct Shape
{
    std::vector<Vector3> Vertexes;
    inCorner inCorner;
    outCorner outCorner;

    Shape (){};
    Shape (std::vector<Vector3> Vertexes)
    {
        Vertexes.assign (Vertexes.begin (), Vertexes.end ());
    }

    void addVertex (Vector3 point) { Vertexes.push_back (point); }

    void setShapeCube (Vector3 &center)
    {
        addVertex (center + Vector3 (-1, 1, 0));
        addVertex (center + Vector3 (1, 1, 0));
        addVertex (center + Vector3 (1, -1, 0));
        addVertex (center + Vector3 (-1, -1, 0));

        inCorner.pos = Vertexes[0];
        outCorner.pos = Vertexes[Vertexes.size() / 2];
    }
};