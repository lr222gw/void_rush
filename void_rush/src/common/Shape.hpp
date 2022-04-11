#include "Vector.hpp"
#include <vector>

struct line
{
    Vector3 point1;
    Vector3 point2;
};

struct Shape
{
    std::vector<Vector3> Vertexes;

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
    }
};