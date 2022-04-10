#include "Platform.hpp"

platform::platform ()
    : pos ({ 0.0f, 0.0f, 0.0f }), obstacles (0), difficluty (0), next (nullptr)
{
}

platform::platform (std::vector<float> pos, int obstacles, int difficluty)
    : obstacles (obstacles), difficluty (difficluty), next (nullptr)
{
    this->pos.x = pos[0];
    this->pos.y = pos[1];
    this->pos.z = pos[2];
}

platform::platform (Vector3 pos, int obstacles, int difficluty)
    : pos (pos), obstacles (obstacles), difficluty (difficluty), next (nullptr)
{
}

platform::~platform () {}

void platform::setPosition (float x, float y, float z)
{
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;
}

void platform::setPosition (Vector3 position) { this->pos = position; }

void platform::move (float xOfset, float yOfset, float zOfset)
{
    this->pos.x += xOfset;
    this->pos.y += yOfset;
    this->pos.z += zOfset;
}

Vector3 platform::getPos () { return this->pos; }

float platform::distance (Vector3 position)
{
    return sqrtf (pow (this->pos.x - position.x, 2)
                  + pow (this->pos.y - position.y, 2)
                  + pow (this->pos.z - position.z, 2));
}

float platform::distance (std::vector<float> &position)
{
    return sqrtf (pow (this->pos.x - position[0], 2.0)
                  + pow (this->pos.y - position[1], 2.0)
                  + pow (this->pos.z - position[2], 2.0));
}
