#include "Shape.hpp"

Shape::Shape(std::vector<vec3> Vertexes)
{
    int test = 5;
    Vertexes.assign(Vertexes.begin(), Vertexes.end());
}

void Shape::addPlane(vec3 a, vec3 b, vec3 c, vec3 d)
{
    planes.push_back(new Plane{ a,b,c,d });
}

void Shape::setShapeCube(vec3& center)
{
    float scale = 1;
    vec3 vert = center;

    std::vector<Plane*> temp_planes;

    Plane* temp_planes_helper[3]{ new XZ_plane(),new XY_plane(),new YZ_plane() };

    for (auto plane : temp_planes_helper) {

        planes.push_back(plane);
        temp_planes.push_back(planes.back());
        Plane* otherSide = new Plane(*plane);
        otherSide->swap_windingorder();
        planes.push_back(otherSide);
        temp_planes.push_back(planes.back());
    }

    int testme = 4;

    //inCorner.pos = Vertexes[0];
    //outCorner.pos = Vertexes[Vertexes.size() / 2];
}

YZ_plane::YZ_plane() { //Clockwise windingorder
    point1 = vec3(0, 1, -1);  // top left
    point2 = vec3(0, 1, 1);  // top right
    point3 = vec3(0, -1, 1);  // bottom right
    point4 = vec3(0, -1, -1);  // bottom left
}

XY_plane::XY_plane() {//Clockwise windingorder
    point1 = vec3(-1, 1, 0); // top left
    point2 = vec3(1, 1, 0);   // top right
    point3 = vec3(1, -1, 0);  // bottom right
    point4 = vec3(-1, -1, 0); // bottom left
}

XZ_plane::XZ_plane() {//Clockwise windingorder        
    point1 = vec3(-1, 0, 1);  // top left
    point2 = vec3(1, 0, 1);   // top right
    point3 = vec3(1, 0, -1);  // bottom right
    point4 = vec3(-1, 0, -1); // bottom left
}

void Plane::swap_windingorder() {
    vec3 temp = this->point2;
    this->point2 = this->point4;
    this->point4 = temp;
}
