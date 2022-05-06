#include <iostream>
#include "Position_generator.hpp"
#include <algorithm>
Position_generator::Position_generator(int _seed)
    : seed(_seed), elements(0), pl(nullptr), startPlat(nullptr), firstJumpPoint(nullptr)
{    
    //this->startPlat = new Platform(Vector3(), 0, 0); //NOTE:  this is now done in the reset function
}

Position_generator::Position_generator (int seed, int elements)
    : seed (seed), elements (elements), pl (nullptr), startPlat(nullptr),firstJumpPoint(nullptr)
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
    float minStepMod = this->AP_conf.minStepMod;
    //float stepMax = pl->getJumpDistance() * platforms_between_anchors;
    float stepMax = this->AP_conf.stepMax;
    float stepMin = stepMax / minStepMod * (int)selectedDiff;
    float distance = 0.0f;
    //float stepMaxZ = pl->jumpHeight() ;// reason for platforms not generating
    float stepMaxZ = this->AP_conf.stepMaxHeight; //pl->jumpHeight();// reason for platforms not generating
    float stepMinZ = this->AP_conf.stepMinHeight; //pl->jumpHeight();// reason for platforms not generating
    vec3 dVect = vec3();
    vec3 position = *this->startPlat->getPos();
    Platform* current = startPlat;
    Platform* newPlat = nullptr;
    //pl->moveto(position); //TODO: this was how we used to do it.
    startPlat->platformShape.setShape(*startPlat->getPos(), AP_conf.stepMax);
    position = current->platformShape.outCorner.pos;
    pl->moveto(position);

    for (int i = 1; i < this->elements; i++)
    {
        dVect.y = randF(stepMinZ, stepMaxZ);
        // dVect.y = (rand() % (2 * stepMax)) - stepMax - 1;
        dVect.y = fmin(dVect.y, stepMaxZ);
        dVect.y = fmax(dVect.y, this->AP_conf.lowest_Height);
        position.y += dVect.y;
        // Generating x and y pos
        dVect.z = randF(this->AP_conf.minZAngle, 1);
        dVect.x = randF(-1, 1);

        // vector3.magnitude  then  vector3.normalizeXY 
        //dVect.normalizeXY();
        float dvect_magnitude = sqrtf(dVect.x * dVect.x + dVect.z * dVect.z);        
        dVect.x = dVect.x / dvect_magnitude;
        dVect.z = dVect.z / dvect_magnitude;    
        // ^^^^^^^^^^^^ vector3.magnitude  then  vector3.normalizeXY 

        
        distance = randF(stepMin, stepMax);
        dVect.x = dVect.x * distance;
        dVect.z = dVect.z* distance;

        position.x += dVect.x;
        position.z += dVect.z;
        // Get random value for Z that is within possible jump


        dvect_magnitude = sqrtf(dVect.x * dVect.x + dVect.z * dVect.z); //Remove?
        if (dvect_magnitude > stepMin && dvect_magnitude < stepMax)
        {
            newPlat = new Platform(position, 0, 1, 0);
            newPlat->platformShape.setShape(*newPlat->getPos(), dVect.length());
           // pl->moveto(*newPlat->getPos()); //TODO: this was how we used to do it.
            pl->moveto(newPlat->platformShape.outCorner.pos);
            position = newPlat->platformShape.outCorner.pos;
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
    pl->reset();

    vec3 position = *this->startPlat->getPos();
    pl->moveto(position); //TODO: this was how we used to do it.
    //startPlat->platformShape.setShapeCube(*startPlat->getPos() + startPlat->platformShape.inCorner.pos); <- Done in Anchor points..
    pl->moveto(startPlat->platformShape.outCorner.pos);
    
    Platform* current = startPlat;
    vec3* startanchorPos = current->getPos();  //TODO: this was how we used to do it.
    //vec3* startanchorPos = &current->platformShape.outCorner.pos;
    vec3* endanchorPos = current->next->getPos(); //TODO: this was how we used to do it.
    //vec3* endanchorPos = &current->next->platformShape.inCorner.pos;
    vec3 dir_between_anchor = *endanchorPos - *startanchorPos;

    Platform* newPlat = nullptr;

    std::vector<Platform*> trashBin;

    Platform* startJumpPoint = nullptr;
    Platform* endJumpPoint = nullptr; 
    MM prev_first_last;
    MM first_last;
    firstJumpPoint = nullptr;
    while (current->next != nullptr) {
        //TODO: Fix trainwreck...
        //current->next->platformShape.setShape(*current->next->getPos() + current->next->platformShape.inCorner.pos);
        startanchorPos = current->getPos(); //TODO: this was how we used to do it.
        //startanchorPos = &current->platformShape.outCorner.pos;
        endanchorPos   = current->next->getPos(); //TODO: this was how we used to do it.        
        //endanchorPos   = &current->next->platformShape.inCorner.pos; 

        pl->moveto(*startanchorPos);

        startJumpPoint = new Platform();        
        startJumpPoint->setPosition(*startanchorPos);
        startJumpPoint->platformShape = current->platformShape;
        trashBin.push_back(startJumpPoint);

        if (endJumpPoint) //endJumpPoint is nullptr, first iteration...
        { 
            endJumpPoint->next = startJumpPoint;
            startJumpPoint->prev = endJumpPoint;
            prev_first_last = first_last;
        }         

        endJumpPoint = new Platform();
        endJumpPoint->setPosition(*endanchorPos); 
        endJumpPoint->platformShape = current->next->platformShape;
        trashBin.push_back(endJumpPoint);

        //first_last = jumpPoint_generation_helper(startJumpPoint, endJumpPoint);
        first_last = jumpPoint_generation_helper(startJumpPoint, endJumpPoint);
        
        if(!firstJumpPoint){
            firstJumpPoint = first_last.first;
        }

        if(prev_first_last.first && prev_first_last.last){
            prev_first_last.last->next = first_last.first;
            first_last.first->prev = prev_first_last.last;
        }

        current = current->next;
    }

    first_last.last->next = nullptr; 

    //Trash handling
    for(Platform* plat :trashBin){
       delete plat;
    }
    trashBin.clear();
}



MM Position_generator::jumpPoint_generation_helper(Platform* start, Platform* end)
{ 
    vec3 middle = (end->platformShape.inCorner.pos - start->platformShape.outCorner.pos) / 2;
    Platform* midd_platform = new Platform() ;
    
    //float distanceToEnd = (end->platformShape.shapeMidpoint - middle).length() ;
    float distanceToEnd = middle.length() ;
    //float distanceToEnd = (end->platformShape.inCorner.pos - middle).length();

    MM ret {nullptr, nullptr};
    midd_platform->setPosition(start->platformShape.outCorner.pos + middle);
    midd_platform->platformShape.setShape(*midd_platform->getPos(), distanceToEnd);
    
    this->jumpPoints.push_back(midd_platform);

    //Create jumppoint between new middle and end if jump not possible
    static int count_M = 0;
    static int count_E = 0;
    //pl->moveto(middle);
    pl->moveto(midd_platform->platformShape.outCorner.pos);
    if(!this->pl->isJumpPossible(end->platformShape.inCorner.pos)){
        
        /*if (this->pl->distance(end->platformShape.inCorner.pos) < pl->getJumpDistance(end->platformShape.inCorner.pos.y) + 2 ) {
            midd_platform->platformShape.setShapeCube(*midd_platform->getPos());
        }else{*/
            jumpPoint_create_offset(midd_platform, *midd_platform->getPos(), start->platformShape.outCorner.pos, end->platformShape.inCorner.pos);
            ret.last = jumpPoint_generation_helper(midd_platform, end).last;
        //}
    }else{
        midd_platform->next = end;
        end->prev = midd_platform;//Set middle.next if end platform is close enogh
        count_M++;
        ret.last = midd_platform;
    }

    //Create jumppoint between start and new middle if jump not possible
    pl->moveto(start->platformShape.outCorner.pos);
    if(!this->pl->isJumpPossible(midd_platform->platformShape.inCorner.pos)){
        
        ret.first = jumpPoint_generation_helper(start, midd_platform).first;
    }else{
        start->next = midd_platform;//Set start.next if new middle platform is close enogh  
        midd_platform->prev = start;
        count_E++;
        ret.first = midd_platform;
    }
    return ret;
}
MM Position_generator::jumpPoint_generation_basic(Platform* start, Platform* end)
{
    vec3 middle = (end->platformShape.inCorner.pos - start->platformShape.outCorner.pos) / 2;
    Platform* midd_platform = new Platform();

    //float distanceToEnd = (end->platformShape.shapeMidpoint - middle).length();
    float distanceToEnd = (*end->getPos() - middle).length();
    

    MM ret{ nullptr, nullptr };
    midd_platform->setPosition(start->platformShape.outCorner.pos + middle);
    midd_platform->platformShape.setShape(*midd_platform->getPos(), distanceToEnd);

    this->jumpPoints.push_back(midd_platform);

    //Create jumppoint between new middle and end if jump not possible
    static int count_M = 0;
    static int count_E = 0;
    
    pl->moveto(midd_platform->platformShape.outCorner.pos);
    
    //vec3 dist_mid_end = end->platformShape.shapeMidpoint - midd_platform->platformShape.shapeMidpoint;
    vec3 dist_mid_end = *end->getPos() - midd_platform->platformShape.shapeMidpoint;
    float rad_sum = end->platformShape.shapeRadius + midd_platform->platformShape.shapeRadius;
    auto l = dist_mid_end.length();
    bool intersects = dist_mid_end.length() < rad_sum*2;

    if (!intersects) {

        
        jumpPoint_create_offset(midd_platform, *midd_platform->getPos(), start->platformShape.outCorner.pos, end->platformShape.inCorner.pos);
        ret.last = jumpPoint_generation_basic(midd_platform, end).last;
        
    }
    else {
        midd_platform->next = end;
        end->prev = midd_platform;//Set middle.next if end platform is close enogh
        count_M++;
        ret.last = midd_platform;
    }

    vec3 dist_mid_start = start->platformShape.shapeMidpoint - midd_platform->platformShape.shapeMidpoint;
    rad_sum = start->platformShape.shapeRadius + midd_platform->platformShape.shapeRadius;

    intersects = dist_mid_start.length() < rad_sum;

    //Create jumppoint between start and new middle if jump not possible
    pl->moveto(start->platformShape.outCorner.pos);
    if (!intersects) {

        ret.first = jumpPoint_generation_basic(start, midd_platform).first;
    }
    else {
        start->next = midd_platform;//Set start.next if new middle platform is close enogh  
        midd_platform->prev = start;
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
    //randomDir.y = 0.f;


    float randomDist = randF(0.f, start_End.length()) / JP_conf.random_dist_dividier;
    vec3 offset = randomDir * randomDist; 

    plat->setPosition(start + offset);
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

Platform*& Position_generator::GetStartPlatform()
{
    return startPlat;
}
