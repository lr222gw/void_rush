#include <iostream>
#include "Position_generator.hpp"

Position_generator::Position_generator(int _seed)
    : seed(_seed), elements(0), pl(nullptr), startPlat(nullptr)
{    
    //this->startPlat = new Platform(Vector3(), 0, 0); //NOTE:  this is now done in the reset function
}

Position_generator::Position_generator (int seed, int elements)
    : seed (seed), elements (elements), pl (nullptr), startPlat(nullptr)
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

    generate_jumpPoints_positions(selectedDiff);

    return true;
}

void Position_generator::generate_anchor_positions(int platforms_between_anchors, Difficulity selectedDiff)
{
    float minStepMod = 2;
    //float stepMax = pl->getJumpDistance() * platforms_between_anchors;
    float stepMax = 200;
    float stepMin = stepMax / minStepMod * (int)selectedDiff;
    float distance = 0.0f;
    float stepMaxZ = pl->jumpHeight() ;// reason for platforms not generating
    vec3 dVect = vec3();
    vec3 position = *this->startPlat->getPos();
    Platform* current = startPlat;
    Platform* newPlat = nullptr;
    pl->moveto(position);
    for (int i = 1; i < this->elements; i++)
    {
        dVect.y = randF(-stepMaxZ, stepMaxZ);
        // dVect.y = (rand() % (2 * stepMax)) - stepMax - 1;
        dVect.y = fmin(dVect.y, stepMaxZ);
        dVect.y = fmax(dVect.y, -100.0f);
        position.y += dVect.y;
        // Using the height the new platform to determine max distance
        //stepMax = pl->getJumpDistance(position.y) * platforms_between_anchors; //the problem is tied to this
        //stepMin = stepMax / minStepMod * (int)selectedDiff;
        // Generating x and y pos
        dVect.z = randF(0, 1);
        dVect.x = randF(-1, 1);

        // vector3.magnitude  then  vector3.normalizeXY 
        //dVect.normalizeXY();
        float dvect_magnitude = sqrtf(dVect.x * dVect.x + dVect.z * dVect.z);        
        dVect.x = dVect.x / dvect_magnitude;
        dVect.z = dVect.z / dvect_magnitude;
        //dvect_magnitude = sqrtf(dVect.x * dVect.x + dVect.z * dVect.z);  //Remove?      
        // ^^^^^^^^^^^^ vector3.magnitude  then  vector3.normalizeXY 

        
        distance = randF(stepMin, stepMax - 3);
        dVect.x = dVect.x * distance;
        dVect.z = dVect.z* distance;

        position.x += dVect.x;
        position.z += dVect.z;
        // Get random value for Z that is within possible jump


        dvect_magnitude = sqrtf(dVect.x * dVect.x + dVect.z * dVect.z); //Remove?
        if (dvect_magnitude > stepMin && dvect_magnitude < stepMax)
        {
            newPlat = new Platform(position, 0, 1, 0);
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

void Position_generator::generate_jumpPoints_positions(Difficulity selectedDiff)
{
    float minStepMod = 2;
    float stepMax = pl->getJumpDistance();
    float stepMin = stepMax / minStepMod * (int)selectedDiff;
    float distance = 0.0f;
    float stepMaxZ = pl->jumpHeight();// reason for platforms not generating
    vec3 dVect = vec3();
    pl->reset();
    vec3 position = *this->startPlat->getPos();
    pl->moveto(position);

#pragma region
    Platform* currentJumpPoint = nullptr;
    Platform* current = startPlat;

    vec3* startanchorPos = current->getPos();
    vec3* endanchorPos = current->next->getPos();

    vec3 dir_between_anchor = *endanchorPos - *startanchorPos;

    //this->jumpPoints.push_back(currentJumpPoint);

#pragma endregion    

    Platform* newPlat;


    Platform* startJumpPoint = nullptr;; //TODO: memory leak    
    Platform* endJumpPoint = nullptr; //TODO: memory leak    
    

    while (current->next != nullptr) {
        startanchorPos = current->getPos();
        endanchorPos   = current->next->getPos();

        pl->moveto(*startanchorPos);
        position = *startanchorPos;

        dir_between_anchor = *endanchorPos - *startanchorPos;
        vec3 normalized_dir = dir_between_anchor.Normalize();
        
        startJumpPoint = new Platform();
        if (endJumpPoint) { endJumpPoint->next = startJumpPoint; } //endJumpPoint is nullptr, first iteration...
        startJumpPoint->setPosition(*startanchorPos);
        this->jumpPoints.push_back(startJumpPoint);        

        endJumpPoint = new Platform();
        endJumpPoint->setPosition(*endanchorPos);        
        this->jumpPoints.push_back(endJumpPoint);
        

        jumpPoint_generation_helper(startJumpPoint, endJumpPoint);
        
        int BREAKPOINT = 3;


        /*
        while (!this->pl->isJumpPossible(*endanchorPos)) {
            //current = current->next;dVect.y = randF(-stepMaxZ, stepMaxZ);
            // dVect.y = (rand() % (2 * stepMax)) - stepMax - 1;
            //dVect.y = fmin(dVect.y, stepMaxZ);
            //dVect.y = fmax(dVect.y, -100.0f);
            dVect.y = 0;
            position.y += dVect.y;
            // Using the height the new platform to determine max distance
            stepMax = pl->getJumpDistance(position.y); //the problem is tied to this
            stepMin = stepMax / minStepMod * (int)selectedDiff;
            // Generating x and y pos
            //dVect.z = randF(0, 1);
            //dVect.x = randF(-1, 1);

            // vector3.magnitude  then  vector3.normalizeXY 
            //dVect.normalizeXZ();
            //float dvect_magnitude = sqrtf(dVect.x * dVect.x + dVect.z * dVect.z);
            dVect.x = dVect.x / dvect_magnitude;
            dVect.z = dVect.z / dvect_magnitude;
            ///dvect_magnitude = sqrtf(dVect.x * dVect.x + dVect.z * dVect.z);  //Remove?      
            // ^^^^^^^^^^^^ vector3.magnitude  then  vector3.normalizeXZ 
            dVect.x = normalized_dir.x;
            dVect.z = normalized_dir.z;

            distance = randF(stepMin, stepMax - 3);
            dVect.x = dVect.x * distance;
            dVect.z = dVect.z * distance;

            position.x += dVect.x;
            position.z += dVect.z;
            // Get random value for Z that is within possible jump


            float dvect_magnitude = sqrtf(dVect.x * dVect.x + dVect.z * dVect.z);//Remove?
            if (currentJumpPoint == nullptr) {
                currentJumpPoint = new Platform(position, 0, 1, 0);
                pl->moveto(*currentJumpPoint->getPos());
            }
            else if (pl->isJumpPossible(position) && dvect_magnitude > stepMin && dvect_magnitude < stepMax)
            {
                newPlat = new Platform(position, 0, 1, 0);
                pl->moveto(*newPlat->getPos());
                currentJumpPoint->next = newPlat;
                this->jumpPoints.push_back(currentJumpPoint);
                currentJumpPoint = newPlat;
            }
            else
            {
                position = position - dVect;
                std::cout << "Jump not possible\n";
            }
        }

        */

        current = current->next;
    }
    endJumpPoint->next = nullptr;
    //this->jumpPoints.push_back(endJumpPoint);
    //this->jumpPoints.push_back(newPlat);
}

void Position_generator::jumpPoint_generation_helper(Platform* start, Platform* end)
{
    vec3 start_end_dist = (*start->getPos() + *end->getPos());
    vec3 middle = (*start->getPos() + *end->getPos()) / 2;
    Platform* midd_platform = new Platform() ;
    midd_platform->setPosition(middle);

    this->jumpPoints.push_back(midd_platform);

    pl->moveto(middle);
    if(!this->pl->isJumpPossible(*end->getPos())){
        
        jumpPoint_generation_helper(midd_platform, end);                
    }else{
        midd_platform->next = end;
    }

    pl->moveto(*start->getPos());
    if(!this->pl->isJumpPossible(*midd_platform->getPos())){
        
        jumpPoint_generation_helper(start, midd_platform);
    }else{
        start->next = midd_platform;
    }

    //return t;
}

void Position_generator::reset_generation(vec3 player_position)
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
    player_position.y = player_position.y - 20; //TODO 
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