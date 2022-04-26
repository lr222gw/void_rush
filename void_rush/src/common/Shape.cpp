#include "Shape.hpp"

Shape::Shape() {}

Shape::~Shape() {
    for (int i = 0; i < planes.size(); i++) {
        delete planes[i];
    }
    planes.clear();
}

void Shape::addPlane(vec3 a, vec3 b, vec3 c, vec3 d)
{
    planes.push_back(new Plane({ a,b,c,d }));
}

void Shape::setShapeCube(vec3& center)
{
    float scale = 1;
    vec3 vert = center;

    std::vector<Plane*> temp_planes;

    Plane* temp_planes_helper[3]{ new XZ_plane(), new XY_plane(), new YZ_plane() };

    for (auto plane : temp_planes_helper) {

        planes.push_back(plane);
        temp_planes.push_back(planes.back());
        Plane* otherside = new Plane(*plane);
        otherside->swap_windingorder();
        planes.push_back(otherside);
        temp_planes.push_back(planes.back());
    }
    for (int i = 0; i < temp_planes.size(); i++) {
        temp_planes[i]->move(center);
    }
    //inCorner.pos = Vertexes[0];
    //outCorner.pos = Vertexes[Vertexes.size() / 2];
}

//definition if plane functions
void Plane::swap_windingorder() {
    vec3 temp = this->point2;
    this->point2 = this->point4;
    this->point4 = temp;
}

void Plane::move(const vec3& ofset) {
    this->point1 = this->point1 + ofset;
    this->point2 = this->point2 + ofset;
    this->point3 = this->point3 + ofset;
    this->point4 = this->point4 + ofset;
}

XZ_plane::XZ_plane() {//Clockwise windingorder        
    point1 = vec3(-1, 0, 1);  // top left
    point2 = vec3(1, 0, 1);   // top right
    point3 = vec3(1, 0, -1);  // bottom right
    point4 = vec3(-1, 0, -1); // bottom left
}

XY_plane::XY_plane() {//Clockwise windingorder
    point1 = vec3(-1, 1, 0); // top left
    point2 = vec3(1, 1, 0);   // top right
    point3 = vec3(1, -1, 0);  // bottom right
    point4 = vec3(-1, -1, 0); // bottom left
}

YZ_plane::YZ_plane() { //Clockwise windingorder
    point1 = vec3(0, 1, -1);  // top left
    point2 = vec3(0, 1, 1);  // top right
    point3 = vec3(0, -1, 1);  // bottom right
    point4 = vec3(0, -1, -1);  // bottom left
}
