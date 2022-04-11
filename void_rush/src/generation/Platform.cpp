#include "Platform.hpp"

platform::platform ()
    : pos ({ 0.0f, 0.0f, 0.0f }), obstacles (0), difficluty (0), rotation (0.0), next (nullptr)
{
    platformShape.setShapeCube (this->pos);
}

platform::platform (std::vector<float> pos, int obstacles, int difficluty, double rotation)
    : obstacles (obstacles), difficluty (difficluty), rotation (rotation), next (nullptr)
{
    this->pos.x = pos[0];
    this->pos.y = pos[1];
    this->pos.z = pos[2];
    platformShape.setShapeCube (this->pos);
}

platform::platform (Vector3 pos, int obstacles, int difficluty, double rotation)
    : pos (pos), obstacles (obstacles), difficluty (difficluty), rotation (rotation), next (nullptr)
{
    platformShape.setShapeCube (this->pos);
}

platform::~platform () {}

void platform::setPosition (float xPos, float yPos, float zPos)
{
    this->pos.x = xPos;
    this->pos.y = yPos;
    this->pos.z = zPos;
}

void platform::setPosition (Vector3 position) { this->pos = position; }

void platform::move (float xOfset, float yOfset, float zOfset)
{
    this->pos.x += xOfset;
    this->pos.y += yOfset;
    this->pos.z += zOfset;
}

Vector3 platform::getPos () { return this->pos; }

float platform::distance (Vector3 position) const
{
    return sqrtf (pow (this->pos.x - position.x, 2)
                  + pow (this->pos.y - position.y, 2)
                  + pow (this->pos.z - position.z, 2));
}

float platform::distance (std::vector<float> &position) const
{
    return sqrtf (pow (this->pos.x - position[0], 2)
                  + pow (this->pos.y - position[1], 2)
                  + pow (this->pos.z - position[2], 2));
}
