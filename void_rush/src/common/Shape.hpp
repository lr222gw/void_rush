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

    void swap_windingorder(){
        vec3 temp = this->point2;
        this->point2 = this->point4;
        this->point4 = temp;
    }    
};


struct XZ_plane : Plane
{
    XZ_plane(){//Clockwise windingorder        
        point1 = vec3(-1, 0, 1);  // top left
        point2 = vec3(1, 0, 1);   // top right
        point3 = vec3(1, 0, -1);  // bottom right
        point4 = vec3(-1, 0, -1); // bottom left
    }    
    //void set_clockwise(){
    //    point1 = vec3(-1, 0, 1);  // top left
    //    point2 = vec3(1, 0, 1);   // top right
    //    point3 = vec3(1, 0, -1);  // bottom right
    //    point4 = vec3(-1, 0, -1); // bottom left
    //}
    //void set_c_clockwise() {
    //    point1 = vec3(-1, 0, 1);  // top left
    //    point4 = vec3(1, 0, 1);   // bottom left
    //    point3 = vec3(1, 0, -1);  // bottom right
    //    point2 = vec3(-1, 0, -1); // top right
    //}
};

struct XY_plane : Plane
{
    XY_plane() {//Clockwise windingorder
        point1 = vec3(-1,  1, 0); // top left
        point2 = vec3( 1,  1, 0);   // top right
        point3 = vec3( 1, -1, 0);  // bottom right
        point4 = vec3(-1, -1, 0); // bottom left
    }
    //void set_clockwise() {
    //    point1 = vec3(-1, 1, 0); // top left
    //    point2 = vec3(1, 1, 0);   // top right
    //    point3 = vec3(1, -1, 0);  // bottom right
    //    point4 = vec3(-1, -1, 0); // bottom left
    //}
    //void set_c_clockwise() {
    //    point1 = vec3(-1, 1, 0); // top left
    //    point4 = vec3(1, 1, 0);   // bottom left
    //    point3 = vec3(1, -1, 0);  // bottom right
    //    point2 = vec3(-1, -1, 0); // top right
    //}
};
struct YZ_plane : Plane
{ 
    YZ_plane() { //Clockwise windingorder
        point1 = vec3(0,  1, -1 );  // top left
        point2 = vec3(0,  1,  1 );  // top right
        point3 = vec3(0, -1,  1 );  // bottom right
        point4 = vec3(0, -1, -1 );  // bottom left
    }
    //void set_clockwise() {
    //    point1 = vec3(0, 1, -1);    // top left
    //    point2 = vec3(0, 1, 1);     // top right
    //    point3 = vec3(0, -1, 1);    // bottom right
    //    point4 = vec3(0, -1, -1);   // bottom left
    //}
    //void set_c_clockwise() {
    //    point1 = vec3(0, 1, -1);    // top left
    //    point4 = vec3(0, 1, 1);     // bottom left
    //    point3 = vec3(0, -1, 1);    // bottom right
    //    point2 = vec3(0, -1, -1);   // top right
    //}
};

struct Normals{
     vec3 up = vec3(0, 1, 0);
     vec3 up = vec3(0, 1, 0);
     vec3 down = vec3(0, -1, 0);
     vec3 right = vec3(0, 0, 1);
     vec3 left = vec3(0, 0, -1);
     vec3 front = vec3(1, 0, 0);
     vec3 back = vec3(-1, 0, 0);
};


struct Shape
{
    std::vector<Plane> planes;
    inCorner inCorner;
    outCorner outCorner;
    static Normals normals;

    Shape (){};
    Shape (std::vector<vec3> Vertexes)
    {
        Vertexes.assign (Vertexes.begin (), Vertexes.end ());
    }

    void addPlane(vec3 a, vec3 b, vec3 c, vec3 d)
    { 
        planes.push_back({a,b,c,d});         
    }

    void setShapeCube (vec3 &center)
    {
        float scale = 1; 
        vec3 vert = center;
        
        std::vector<Plane&> temp_planes;
        
        Plane temp_planes_helper[3]{ XZ_plane(), XY_plane(), YZ_plane()};                

        for(auto plane : temp_planes_helper){
            
            planes.push_back(plane);
            temp_planes.push_back(planes.back());
            plane.swap_windingorder();
            planes.push_back(plane);
            temp_planes.push_back(planes.back());
        }



        //inCorner.pos = Vertexes[0];
        //outCorner.pos = Vertexes[Vertexes.size() / 2];
    }
};