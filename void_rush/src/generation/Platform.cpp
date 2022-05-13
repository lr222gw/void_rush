#include "Platform.hpp"

Platform::Platform()
    : pos ({ 0.0f, 0.0f, 0.0f }), obstacles (0), difficluty (0),
    next (nullptr), prev(nullptr)
{
 
}

Platform::Platform(vec3 pos, int obstacles, int difficluty)
    : pos(pos), obstacles(obstacles), difficluty(difficluty), 
    next(nullptr), prev(nullptr)
{}

Platform::~Platform () {

}

void Platform::setPosition (float xPos, float yPos, float zPos)
{
    this->pos.x = xPos;
    this->pos.y = yPos;
    this->pos.z = zPos;

    platformShape.setPosition(vec3(xPos, yPos, zPos));
}

void Platform::setPosition (vec3 position) { 
    this->pos = position; 
    platformShape.move(position);
}

void Platform::move (float xOfset, float yOfset, float zOfset)
{
    this->pos.x += xOfset;
    this->pos.y += yOfset;
    this->pos.z += zOfset;
    platformShape.move(vec3(xOfset, yOfset, zOfset));
}

vec3* Platform::getPos () { return &this->pos; }

float Platform::distance (vec3* position) const
{
    return sqrtf (  powf (this->pos.x - position->x, 2)
                  + powf (this->pos.y - position->y, 2)
                  + powf (this->pos.z - position->z, 2));
}

float Platform::distance (std::vector<float> &position) const
{
    return sqrtf (powf (this->pos.x - position[0], 2)
                  + powf (this->pos.y - position[1], 2)
                  + powf (this->pos.z - position[2], 2));
}
