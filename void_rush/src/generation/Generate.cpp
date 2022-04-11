#include "Generate.hpp"
#include <iostream>

generation::generation (int seed, int elements)
    : seed (seed), elements (elements), pl (nullptr)
{
    this->startPlat = new platform (Vector3 (), 0, 0);
}

generation::~generation ()
{
    this->startPlat = nullptr;
    for (int i = 0; i < anchors.size (); i++)
    {
        delete anchors.at (i);
    }
    anchors.clear ();
}

bool generation::start (int selectedDiff)
{
    srand (this->seed);
    Vector3 dVect = Vector3 ();
    Vector3 position = this->startPlat->getPos ();
    float minStepMod = 2;
    float stepMax = pl->getJumpDistance ();
    float stepMin = pl->getJumpDistance () / minStepMod * selectedDiff;
    float distance = 0.0f;
    float stepMaxZ = pl->jumpHeight ();
    platform *current = startPlat;
    platform *newPlat = nullptr;
    for (int i = 0; i < this->elements; i++)
    {
        dVect.z = randF (-stepMaxZ, stepMaxZ);
        // dVect.z = (rand() % (2 * stepMax)) - stepMax - 1;
        dVect.z = fmin (dVect.z, stepMaxZ);
        position.z += dVect.z;
        // Using the height the new platform to determine max distance
        stepMax = pl->getJumpDistance (position.z);
        stepMin = stepMax / minStepMod * selectedDiff;
        // Generating x and y pos
        dVect.x = randF (0, 1);
        dVect.y = randF (0, 1);
        dVect.normalizeXY ();
        distance = randF (stepMin, stepMax - 3);
        dVect.x *= distance;
        dVect.y *= distance;

        position.x += dVect.x;
        position.y += dVect.y;

        // Get random value for Z that is within possible jump

        if (this->pl->isJumpPossible (position) && dVect.magnitude () > stepMin
            && dVect.magnitude () < stepMax)
        {
            newPlat = new platform (position, 0, 1);
            pl->moveto (newPlat->getPos ());
            current->next = newPlat;
            this->anchors.push_back (current);
            current = newPlat;
        }
        else
        {
            i -= 1;
            position -= dVect;
            std::cout << "Jump not possible\n";
        }
    }
    this->anchors.push_back (newPlat);
    return true;
}

std::vector<platform *> generation::getPlatforms () { return this->anchors; }

void generation::assignPlayer (player *player) { this->pl = player; }

float generation::randF (float min, float max)
{
    float random = min + ((float)rand ()) / ((float)RAND_MAX / (max - min));
    // std::cout << "random float " << random << std::endl;
    return random;
}