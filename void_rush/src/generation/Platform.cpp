#include "Platform.hpp"

Platform::Platform()
    : pos ({ 0.0f, 0.0f, 0.0f }), obstacles (0), difficluty (0),
      rotation (0.0), next (nullptr)
{
    platformShape.setShapeCube (this->pos);
    rotMat.rotateAroundZ (this->rotation);
    rotMat.rotateAroundZ (this->rotation);
    for (int i = 0; i < this->platformShape.Vertexes.size (); i++)
    {
        rotMat.MultiplyVector (platformShape.Vertexes.at (i));
    }
}

Platform::Platform(std::vector<float> pos, int obstacles, int difficluty,
                    double rotation)
    : obstacles (obstacles), difficluty (difficluty), rotation (rotation),
      next (nullptr)
{
    this->pos.x = pos[0];
    this->pos.y = pos[1];
    this->pos.z = pos[2];
    platformShape.setShapeCube (this->pos);
    rotMat.rotateAroundZ (this->rotation);
    rotMat.rotateAroundZ (this->rotation);
    for (int i = 0; i < this->platformShape.Vertexes.size (); i++)
    {
        rotMat.MultiplyVector (platformShape.Vertexes.at (i));
    }
}

Platform::Platform(vec3 pos, int obstacles, int difficluty,
                    double rotation)
    : pos (pos), obstacles (obstacles), difficluty (difficluty),
      rotation (rotation), next (nullptr)
{
    this->platformShape.setShapeCube (this->pos);
    rotMat.rotateAroundZ (this->rotation);
    for (int i = 0; i < this->platformShape.Vertexes.size (); i++)
    {
        rotMat.MultiplyVector (platformShape.Vertexes.at (i));
    }
}

Platform::~Platform () {}

void Platform::setPosition (float xPos, float yPos, float zPos)
{
    this->pos.x = xPos;
    this->pos.y = yPos;
    this->pos.z = zPos;
}

void Platform::setPosition (vec3 position) { this->pos = position; }

void Platform::move (float xOfset, float yOfset, float zOfset)
{
    this->pos.x += xOfset;
    this->pos.y += yOfset;
    this->pos.z += zOfset;
}

vec3* Platform::getPos () { return &this->pos; }

double Platform::getRotation () const { return this->rotation; }

float Platform::distance (vec3* position) const
{
    return sqrtf (  pow (this->pos.x - position->x, 2)
                  + pow (this->pos.y - position->y, 2)
                  + pow (this->pos.z - position->z, 2));
}

float Platform::distance (std::vector<float> &position) const
{
    return sqrtf (pow (this->pos.x - position[0], 2)
                  + pow (this->pos.y - position[1], 2)
                  + pow (this->pos.z - position[2], 2));
}
