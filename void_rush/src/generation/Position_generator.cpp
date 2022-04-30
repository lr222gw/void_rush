#include <iostream>
#include "Position_generator.hpp"
#include <algorithm>
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
    float stepMax = 50;
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
            newPlat->prev = current;
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

    Platform* newPlat = nullptr;


    Platform* startJumpPoint = nullptr;; //TODO: memory leak    
    Platform* endJumpPoint = nullptr; //TODO: memory leak    
    MM* prev_first_last = nullptr;
    MM first_last;
    firstJumpPoint = nullptr;
    while (current->next != nullptr) {
        startanchorPos = current->getPos();
        endanchorPos   = current->next->getPos();

        pl->moveto(*startanchorPos);
        position = *startanchorPos;

        dir_between_anchor = *endanchorPos - *startanchorPos;
        vec3 normalized_dir = dir_between_anchor.Normalize();


        startJumpPoint = new Platform();        
        if (endJumpPoint) { 
            endJumpPoint->next = startJumpPoint;
            startJumpPoint->prev = endJumpPoint;
            prev_first_last = new MM(first_last);
        } //endJumpPoint is nullptr, first iteration...
        startJumpPoint->setPosition(*startanchorPos);
        //this->jumpPoints.push_back(startJumpPoint);        

        endJumpPoint = new Platform();
        endJumpPoint->setPosition(*endanchorPos);           
        //this->jumpPoints.push_back(endJumpPoint);
        
    /*    if (prev_last_jumppoint) {
            prev_last_jumppoint->next = first_jumppoint;
        }*/

        first_last = jumpPoint_generation_helper(startJumpPoint, endJumpPoint);
        
        if(!firstJumpPoint){
            firstJumpPoint = first_last.first;
        }

        if(prev_first_last){
            prev_first_last->last->next = first_last.first;
            first_last.first->prev = prev_first_last->last;
        }

        int BREAKPOINT = 3;



        current = current->next;
    }
    endJumpPoint->next = nullptr; //TODO: should not be needed...
    //this->jumpPoints.push_back(endJumpPoint);
    //this->jumpPoints.push_back(newPlat);
    first_last.last->next = endJumpPoint->next;
}



MM Position_generator::jumpPoint_generation_helper(Platform* start, Platform* end)
{    
    vec3 start_end_dist = (*start->getPos() + *end->getPos()); //TODO: remove
    vec3 middle = (*end->getPos() - *start->getPos()) / 2;
    Platform* midd_platform = new Platform() ;
    MM ret {nullptr, nullptr};
    midd_platform->setPosition(*start->getPos() + middle);
    
    this->jumpPoints.push_back(midd_platform);

    //Create jumppoint between new middle and end if jump not possible
    static int count_M = 0;
    static int count_E = 0;
    //pl->moveto(middle);
    pl->moveto(*midd_platform->getPos());
    if(!this->pl->isJumpPossible(*end->getPos())){
        jumpPoint_create_offset(midd_platform, *midd_platform->getPos(), *start->getPos(), *end->getPos());
        ret.last = jumpPoint_generation_helper(midd_platform, end).last;                
    }else{
        midd_platform->next = end;
        end->prev = midd_platform;//Set middle.next if end platform is close enogh
        count_M++;
        ret.last = midd_platform;
    }

    //Create jumppoint between start and new middle if jump not possible
    pl->moveto(*start->getPos());
    if(!this->pl->isJumpPossible(*midd_platform->getPos())){
        
        ret.first = jumpPoint_generation_helper(start, midd_platform).first;
    }else{
        start->next = midd_platform;//Set start.next if new middle platform is close enogh  
        midd_platform->prev = start;
        count_E++;
        ret.first = midd_platform;
    }
    return ret;
}
MM Position_generator::jumpPoint_generation_helper(vec3 start, vec3 end){
    vec3 start_end_dist = (start + end); //TODO: remove
    vec3 middle = (end - start) / 2;
    Platform* midd_platform = new Platform();
    MM ret{ nullptr, nullptr };
    midd_platform->setPosition(start + middle);

    this->jumpPoints.push_back(midd_platform);

    //Create jumppoint between new middle and end if jump not possible
    static int count_M = 0;
    static int count_E = 0;
    //pl->moveto(middle);
    pl->moveto(*midd_platform->getPos());
    if (!this->pl->isJumpPossible(end)) {
        jumpPoint_create_offset(midd_platform, *midd_platform->getPos(), start, end);
        ret.last = jumpPoint_generation_helper(*midd_platform->getPos(), end).last;
    }
    else {
        midd_platform->next = ret.last; //Set middle.next if end platform is close enogh
        count_M++;
        ret.last = midd_platform;
    }

    //Create jumppoint between start and new middle if jump not possible
    pl->moveto(start);
    if (!this->pl->isJumpPossible(*midd_platform->getPos())) {

        ret.first = jumpPoint_generation_helper(start, *midd_platform->getPos()).last;
    }
    else {
        ret.first->next = midd_platform;//Set start.next if new middle platform is close enogh      
        count_E++;
        ret.first = midd_platform;
    }
    return ret;
}
//TODO: remove return value...
vec3 Position_generator::jumpPoint_create_offset(Platform* plat,vec3& currentMiddle, vec3 start, vec3 end)
{
    vec3 temp = vec3(randF(-1.f, 1.f), randF(-1.f, 1.f), randF(-1.f, 1.f)).Normalize();
    vec3 start_End = (end - start);
    vec3 start_End_dir = vec3::Normalize(start_End);

    while(  start_End_dir * temp > JP_conf.rand_dir_max_angle_percent || 
            start_End_dir * temp < JP_conf.rand_dir_min_angle_percent)
    {
        //to get all possible directions, use -1 and 1...
        temp = vec3(randF(-1.f, 1.f), randF(-1.f, 1.f), randF(-1.f, 1.f)).Normalize();
    }    
        
    vec3 randomDir = start_End_dir.X(temp).Normalize();
    randomDir.y = std::clamp(randomDir.y, JP_conf.y_min_clamp, JP_conf.y_max_clamp);


    float randomDist = randF(0.f, start_End.length()) / JP_conf.random_dist_dividier;
    vec3 offset = randomDir * randomDist; 

    plat->setPosition(currentMiddle + offset);        
    return offset;
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

    if(imgui_conf.useOrigo){
        this->startPlat = new Platform(vec3(0,-10,0), 0, 0);
    }else{
        this->startPlat = new Platform(player_position, 0, 0);
    }
    
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