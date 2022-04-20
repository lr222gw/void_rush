#include <iostream>
#include "Position_generator.hpp"

Position_generator::Position_generator(int _seed)
    : seed(_seed), elements(0), pl(nullptr)
{    
    //this->startPlat = new Platform(Vector3(), 0, 0); //NOTE:  this is now done in the reset function
}

Position_generator::Position_generator (int seed, int elements)
    : seed (seed), elements (elements), pl (nullptr)
{
    //this->startPlat = new Platform (Vector3 (), 0, 0); //NOTE:  this is now done in the reset function
}

Position_generator::~Position_generator ()
{
    this->startPlat = nullptr; //TODO: ask nils; do we not need to delete startPlat?
    for (int i = 0; i < anchors.size (); i++)
    {
        delete anchors.at (i);
    }
    for (int i = 0; i < this->jumpPoints.size (); i++)
    {
        delete this->jumpPoints.at (i);
    }
    anchors.clear ();
    jumpPoints.clear();
}

bool Position_generator::start (Difficulity selectedDiff)
{

    static int platforms_between_anchors = (3 + 1);

    srand (this->seed);   
    
    generate_anchor_positions(platforms_between_anchors, selectedDiff);

    generate_jumpPoints_positions(platforms_between_anchors);

    return true;
}

void Position_generator::generate_anchor_positions(int platforms_between_anchors, Difficulity selectedDiff)
{
    float minStepMod = 2;
    float stepMax = pl->getJumpDistance() * platforms_between_anchors;
    float stepMin = pl->getJumpDistance() / minStepMod * (int)selectedDiff;
    float distance = 0.0f;
    float stepMaxZ = pl->jumpHeight();
    float rotation = 0.0f;
    vec3 dVect = vec3();
    vec3 position = *this->startPlat->getPos();
    Platform* current = startPlat;
    Platform* newPlat = nullptr;
    for (int i = 0; i < this->elements; i++)
    {
        dVect.z = randF(-stepMaxZ, stepMaxZ);
        // dVect.z = (rand() % (2 * stepMax)) - stepMax - 1;
        dVect.z = fmin(dVect.z, stepMaxZ);
        position.z += dVect.z;
        // Using the height the new platform to determine max distance
        stepMax = pl->getJumpDistance(position.z) * platforms_between_anchors;
        stepMin = stepMax / minStepMod * (int)selectedDiff;
        // Generating x and y pos
        dVect.x = randF(0, 1);
        dVect.y = randF(-1, 1);

        // vector3.magnitude  then  vector3.normalizeXY 
        //dVect.normalizeXY();
        float dvect_magnitude = sqrtf(dVect.x * dVect.x + dVect.y * dVect.y);        
        dVect.x = dVect.x / dvect_magnitude;
        dVect.y = dVect.y / dvect_magnitude;
        dvect_magnitude = sqrtf(dVect.x * dVect.x + dVect.y * dVect.y);        
        // ^^^^^^^^^^^^ vector3.magnitude  then  vector3.normalizeXY 

        
        distance = randF(stepMin, stepMax - 3);
        dVect.x = dVect.x * distance;
        dVect.y = dVect.y* distance;

        position.x += dVect.x;
        position.y += dVect.y;
        rotation = randF(0, 90);
        // Get random value for Z that is within possible jump


        dvect_magnitude = sqrtf(dVect.x * dVect.x + dVect.y * dVect.y);
        if (this->pl->isJumpPossible(position) && dvect_magnitude > stepMin
            && dvect_magnitude < stepMax)
        {
            newPlat = new Platform(position, 0, 1, rotation);
            pl->moveto(*newPlat->getPos());
            current->next = newPlat;
            this->anchors.push_back(current);
            current = newPlat;
        }
        else
        {
            i -= 1;
            position = position - dVect;
            std::cout << "Jump not possible\n";
        }
    }
    this->anchors.push_back(newPlat);
}

void Position_generator::generate_jumpPoints_positions(int platforms_between_anchors)
{
#pragma region
    Platform* currentJumpPoint;
    Platform* current = startPlat;

    vec3* startanchorPos = current->getPos();
    vec3* endanchorPos = current->next->getPos();

    vec3 dir_between_anchor = *endanchorPos - *startanchorPos;

    vec3 pos_to_place_jump_point = *startanchorPos +
        (dir_between_anchor / (platforms_between_anchors - 1));


    currentJumpPoint = new Platform(pos_to_place_jump_point, 0, 1, 0);

    this->jumpPoints.push_back(currentJumpPoint);

#pragma endregion    

    Platform* newPlat;

    while (current->next != nullptr) {
        startanchorPos = current->getPos();
        endanchorPos = current->next->getPos();

        dir_between_anchor = *endanchorPos - *startanchorPos;

        for (int i = 1; i < platforms_between_anchors; i++) {
            if (this->jumpPoints[0]->next == nullptr) { //TODO: Optimera bort denna senare
                i += 1;
            }
            pos_to_place_jump_point = *startanchorPos +
                (dir_between_anchor / (platforms_between_anchors - 1) * i);

            newPlat = new Platform(pos_to_place_jump_point, 0, 1, 0);
            currentJumpPoint->next = newPlat;
            this->jumpPoints.push_back(newPlat);
            currentJumpPoint = newPlat;
        }

        current = current->next;
    }
}

void Position_generator::reset_anchors(vec3 player_position)
{
    for (Platform* anchor : this->anchors) {
        delete anchor;
    }
    anchors.clear();
    for (int i = 0; i < this->jumpPoints.size (); i++)
    {
        delete this->jumpPoints.at (i);
    }
    this->jumpPoints.clear();

    this->startPlat = new Platform(player_position, 0, 0);
    
    this->pl->reset();
}

void Position_generator::set_seed(int _seed)
{
    this->seed = _seed;
}

std::vector<Platform *>* Position_generator::getAnchors () { return &this->anchors; }

std::vector<Platform*>* Position_generator::getJumpPoints()
{
    return &this->jumpPoints;
}

void Position_generator::setNrOfElements(int nrOfElements)
{
    this->elements = nrOfElements;
}

void Position_generator::assignPlayer (Player_jump_checker* player) { this->pl = player; }

float Position_generator::randF (float min, float max)
{
    float random = min + ((float)rand ()) / ((float)RAND_MAX / (max - min));
    // std::cout << "random float " << random << std::endl;
    return random;
}