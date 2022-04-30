#pragma once
#include "Vector.hpp"
#include "3Dproj/Vec.h"
#include "Helper.hpp"
#include <vector>

class Shape_exporter;
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
    Plane();
    Plane(vec3 a, vec3 b, vec3 c, vec3 d);
    vec3 point1;
    vec3 point2;
    vec3 point3;
    vec3 point4;
    vec3 normal;
    vec3 offset;
    vec2 uv[4];
    
    void swap_windingorder();
    const vec3 get_normal();;
    const vec3 get_rot();;
    void move(const vec3& ofset);
    vec3 get_center();
    std::vector<vec3*> get_all_points();    
protected:
    void update_normal();;
};


struct XZ_plane : public Plane
{
    XZ_plane(vec3 scale);    
};

struct XY_plane : public Plane
{
    XY_plane(vec3 scale);
};
struct YZ_plane : public Plane
{ 
    YZ_plane(vec3 scale);
};

struct Normals{
     vec3 up =  vec3(0, 1, 0);
     vec3 down = vec3(0, -1, 0);
     vec3 right = vec3(0, 0, 1);
     vec3 left = vec3(0, 0, -1);
     vec3 front = vec3(1, 0, 0);
     vec3 back = vec3(-1, 0, 0);
};
static Normals normals;

struct vec3_pair {
    vec3 first;
    vec3 second;
};

class Shape
{
public:
    Shape();
    ~Shape();
    void addPlane(vec3 a, vec3 b, vec3 c, vec3 d);
    void setPosition(vec3 pos);
    void setScale(vec3 scale);
    void setShape(vec3 center);
    void setShapeCube(vec3 center);

    void export_as_obj();

    std::vector<Plane*> planes;
    inCorner inCorner;
    outCorner outCorner;
    static Normals normals;
    vec3 scale;

    //std::vector<DirectX::XMFLOAT4*> bounding_boxes; //TODO: Handle memory here!
    //std::vector<vec3[2]> bounding_boxes; //TODO: Handle memory here!
    std::vector<vec3_pair> bounding_boxes; //TODO: Handle memory here!

    

};