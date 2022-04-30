#include "Platform.hpp"

Platform::Platform()
    : pos ({ 0.0f, 0.0f, 0.0f }), obstacles (0), difficluty (0),
      rotation (0.0), next (nullptr), prev(nullptr)
{
    //platformShape.setShapeCube (this->pos);
    rotMat.rotateAroundZ (this->rotation);
    rotMat.rotateAroundZ (this->rotation);
}

Platform::Platform(std::vector<float> pos, int obstacles, int difficluty,
                    float rotation)
    : obstacles (obstacles), difficluty (difficluty), rotation (rotation),
      next (nullptr), prev(nullptr)
{
    this->pos.x = pos[0];
    this->pos.y = pos[1];
    this->pos.z = pos[2];
    //platformShape.setShapeCube (this->pos);
    rotMat.rotateAroundZ (this->rotation);
    rotMat.rotateAroundZ (this->rotation);
}

Platform::Platform(vec3 pos, int obstacles, int difficluty,
                    float rotation)
    : pos (pos), obstacles (obstacles), difficluty (difficluty),
      rotation (rotation), next (nullptr), prev(nullptr)
{
    //this->platformShape.setShapeCube (this->pos);
    rotMat.rotateAroundZ (this->rotation);
}

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
    platformShape.setPosition(position);
}

void Platform::move (float xOfset, float yOfset, float zOfset)
{
    this->pos.x += xOfset;
    this->pos.y += yOfset;
    this->pos.z += zOfset;
}

vec3* Platform::getPos () { return &this->pos; }

float Platform::getRotation () const { return this->rotation; }

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
