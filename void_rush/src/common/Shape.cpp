#include "Shape.hpp"
#include "Shape_exporter.hpp"

Shape::Shape():scale(1,1,1) {}

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

void Shape::setPosition(vec3 pos)
{
    for (int i = 0; i < planes.size(); i++) {
        vec3 offset = vec3::Normalize(planes[i]->get_normal()).mul(vec3(1, 1, 1));
        planes[i]->move(pos + offset);
    }
}

void Shape::setScale(vec3 scale)
{
    this->scale = scale;
}

void Shape::setShapeCube(vec3& center)
{    
    vec3 vert = center;

    std::vector<Plane*> temp_planes;

    Plane* temp_planes_helper[3]{ new XZ_plane(scale), new XY_plane(scale), new YZ_plane(scale) };

    for (auto plane : temp_planes_helper) {
        
        planes.push_back(plane);
        temp_planes.push_back(planes.back());

        Plane* otherside = new Plane(*plane);
        otherside->swap_windingorder();            
        planes.push_back(otherside);
        temp_planes.push_back(planes.back());
    }
    for (int i = 0; i < temp_planes.size(); i++) {
        temp_planes[i]->offset = vec3::Normalize(temp_planes[i]->get_normal()).mul(scale);
        temp_planes[i]->move(center + temp_planes[i]->offset); //TODO: remove?
    }
    //inCorner.pos = Vertexes[0];
    //outCorner.pos = Vertexes[Vertexes.size() / 2];
}

void Shape::export_as_obj()
{
    //TODO: this could probably be removed if we wanted...
    //Shape_exporter::export_all(this, "test");

}

//definition if plane functions
void Plane::swap_windingorder() {
    vec3 temp = this->point2;
    this->point2 = this->point4;
    this->point4 = temp;
    update_normal();
}

const vec3 Plane::get_normal() { return normal; }

const vec3 Plane::get_rot() { 

    vec3 ret = this->get_normal(); 
    if(ret.x != 0){
        ret.x = ret.x == 1 ? deg_to_rad(90) : deg_to_rad(180 + 90);
        
    }
    if(ret.y != 0){
        if(ret.y == -1 ){
            ret.x = deg_to_rad(180);
        }
        ret.y = 0;
    }
    
    if(ret.z != 0){
        ret.z = ret.z == 1 ? deg_to_rad(90) : deg_to_rad(180 + 90);
        
    }

    return ret;

}

void Plane::move(const vec3& ofset) {
    this->point1 = this->point1 + ofset;
    this->point2 = this->point2 + ofset;
    this->point3 = this->point3 + ofset;
    this->point4 = this->point4 + ofset;
}

vec3 Plane::get_center()
{
    return (point1+point2+point3+point4)/4;
}

std::vector<vec3*> Plane::get_all_points()
{
    return std::vector<vec3*>{ &point1, & point2, & point3, & point4 };
}

void Plane::update_normal() { 
    normal = vec3((point2 - point1)).X(point4 - point1).Normalize();
}

XZ_plane::XZ_plane(vec3 scale) {//Clockwise windingorder        
    
    point1 = vec3(-1, 0, 1 ).mul(scale) ;  // top left
    point2 = vec3( 1, 0,  1).mul(scale);   // top right
    point3 = vec3( 1, 0, -1).mul(scale);  // bottom right
    point4 = vec3(-1, 0, -1).mul(scale); // bottom left
    update_normal();
}

XY_plane::XY_plane(vec3 scale) {//Clockwise windingorder
    
    point1 = vec3(-1,  1, 0).mul(scale); // top left
    point2 = vec3( 1,  1, 0).mul(scale);   // top right
    point3 = vec3( 1, -1, 0).mul(scale);  // bottom right
    point4 = vec3(-1, -1, 0).mul(scale); // bottom left
    update_normal();
}

YZ_plane::YZ_plane(vec3 scale) { //Clockwise windingorder
    point1 = vec3(0,  1, -1).mul(scale) ;  // top left
    point2 = vec3(0,  1,  1).mul(scale) ;  // top right
    point3 = vec3(0, -1,  1).mul(scale) ;  // bottom right
    point4 = vec3(0, -1, -1).mul(scale) ;  // bottom left
    update_normal();
}
