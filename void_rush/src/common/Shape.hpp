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

struct Plane
{
    vec3 point1;
    vec3 point2;
    vec3 point3;
    vec3 point4;

    void swap_windingorder();

    void move(const vec3& ofset);
};


struct XZ_plane : Plane
{
    XZ_plane();

};

struct XY_plane : Plane
{
    XY_plane();

};
struct YZ_plane : Plane
{ 
    YZ_plane();

};

struct Normals{
     vec3 up = vec3(0, 1, 0);
     vec3 down = vec3(0, -1, 0);
     vec3 right = vec3(0, 0, 1);
     vec3 left = vec3(0, 0, -1);
     vec3 front = vec3(1, 0, 0);
     vec3 back = vec3(-1, 0, 0);
};


struct Shape
{
    std::vector<Plane*> planes;
    inCorner inCorner;
    outCorner outCorner;
    static Normals normals;

    Shape (){};

    ~Shape();

    void addPlane(vec3 a, vec3 b, vec3 c, vec3 d);

    void setShapeCube(vec3& center);
};