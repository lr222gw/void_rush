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
    vec3 normal;

    void swap_windingorder();
    const vec3 get_normal() { return normal; };
    void move(const vec3& ofset);
protected:
    void update_normal() { normal = vec3((point2 - point1 ).X(point4 - point1)); };
};


struct XZ_plane : Plane
{
    XZ_plane(vec3 scale);    
};

struct XY_plane : Plane
{
    XY_plane(vec3 scale);
};
struct YZ_plane : Plane
{ 
    YZ_plane(vec3 scale);
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

    Shape();

    ~Shape();

    void addPlane(vec3 a, vec3 b, vec3 c, vec3 d);

    void setShapeCube(vec3& center);
};