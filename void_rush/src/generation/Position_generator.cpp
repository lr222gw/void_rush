#include <iostream>
#include "Position_generator.hpp"
#include <algorithm>
Position_generator::Position_generator(int _seed)
    : seed(_seed), pl(nullptr), startPlat(nullptr), firstJumpPoint(nullptr)
{    
}

Position_generator::~Position_generator ()
{
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
    srand (this->seed);   
    
    generate_anchor_positions(selectedDiff);
    generate_jumpPoints_positions(selectedDiff);
    removeOverlappingPlatformVoxels();

    return true;
}

void Position_generator::generate_anchor_positions(Difficulity selectedDiff)
{
    float minStepMod = this->AP_conf.minStepMod;
    float stepMax = this->AP_conf.stepMax;
    float stepMin = stepMax / minStepMod * (int)selectedDiff;
    float distance = 0.0f;
    float stepMaxZ = this->AP_conf.stepMaxHeight; 
    float stepMinZ = this->AP_conf.stepMinHeight; 

    vec3 dVect = vec3();
    vec3 position = *this->startPlat->getPos();

    Platform* current = startPlat;
    Platform* newPlat = nullptr;

    startPlat->platformShape.setAnchorShape(*startPlat->getPos(), AP_conf.stepMax);
    position = current->platformShape.outCorner.pos;
    pl->moveto(position);

    for (int i = 1; i < this->AP_conf.nrOfAnchors; i++)
    {
        stepMaxZ = this->AP_conf.stepMaxHeight;
        stepMinZ = this->AP_conf.stepMinHeight;
        
        if( rand() % this->AP_conf.freeFallRate == 0 ){ // Determine if next anchor is for freefall
            stepMinZ += this->AP_conf.freeFallModifier;
            stepMaxZ += this->AP_conf.freeFallModifier;
        }

        dVect.y = randF(stepMinZ, stepMaxZ);
        dVect.y = fmin(dVect.y, stepMaxZ);
        dVect.y = fmax(dVect.y, this->AP_conf.lowest_Height);
        position.y += dVect.y;

        // Generating x and y pos
        dVect.z = randF(this->AP_conf.minZAngle, 1);
        dVect.x = randF(-1, 1);

        dVect.Normalize_XZ();
        
        distance = randF(stepMin, stepMax);
        dVect.x = dVect.x * distance;
        dVect.z = dVect.z* distance;

        position.x += dVect.x;
        position.z += dVect.z;

        float dvect_magnitude = dVect.length_XZ();
        if (dvect_magnitude > stepMin && dvect_magnitude < stepMax)
        {
            newPlat = new Platform(position);            
            newPlat->platformShape.setAnchorShape(*newPlat->getPos(), dVect.length());

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

    Platform* current = startPlat;
    vec3* startanchorPos = current->getPos();
    vec3* endanchorPos = current->next->getPos(); 

    std::vector<Platform*> trashBin;

    Platform* startJumpPoint = nullptr;
    Platform* endJumpPoint = nullptr;
    this->firstJumpPoint = nullptr;

    FirstLast_between_Anchor prev_first_last;
    FirstLast_between_Anchor first_last;

    //Iterate over recursive function to generate jumppoints using midplace alg.
    while (current->next != nullptr) {
        startanchorPos = current->getPos(); 
        endanchorPos   = current->next->getPos(); 

        pl->moveto(*startanchorPos);

        //Creates temporary Plattform to fake anchors position
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

        //Generates the jumppoints between start and end Anchor pos
        first_last = jumpPoint_generation_helper(startJumpPoint, endJumpPoint);
        
        
        if(!firstJumpPoint){ //firstJumpPoint is nullptr, first iteration...
            firstJumpPoint = first_last.first;
        }

        //Link end of previous jumpPoint chain to start of current jumppoint chain
        if(prev_first_last.first && prev_first_last.last){
            prev_first_last.last->next = first_last.first;
            first_last.first->prev = prev_first_last.last;
        }

        current = current->next;
    }

    first_last.last->next = nullptr;  //last.next will have junk value, so set to nullptr

    //Trash handling
    for(Platform* plat :trashBin){
       delete plat;
    }
    trashBin.clear();
}



FirstLast_between_Anchor Position_generator::jumpPoint_generation_helper(Platform* start, Platform* end)
{ 

    vec3 middle = (end->platformShape.inCorner.pos - start->platformShape.outCorner.pos) / 2;
    float distanceToEnd = middle.length() ;

    FirstLast_between_Anchor ret {nullptr, nullptr};

    Platform* midd_platform = new Platform() ;    
    midd_platform->setPosition(start->platformShape.outCorner.pos + middle);
    //auto t = start->platformShape.previousVoxels[0].current_center - *midd_platform->getPos();

    
    
    ///
    midd_platform->platformShape.setJumppointShape(*midd_platform->getPos(), distanceToEnd, &start->platformShape);


    pl->moveto(midd_platform->platformShape.outCorner.pos);
    
    this->jumpPoints.push_back(midd_platform);    

    //////////////////////////


  //  midd_platform->platformShape.outCorner.pos.x < end->platformShape.inCorner.pos.x &&

        //if(abs(end->platformShape.inCorner.pos.x) - abs(midd_platform->platformShape.outCorner.pos.x) >= 0) {}
        //for(){
        
//        }


    //////////////////////////

    //Create jumppoint between new middle and end if jump not possible    
    if(!this->pl->isJumpPossible(end->platformShape.inCorner.pos)){
                
        jumpPoint_create_offset(midd_platform, *midd_platform->getPos(), 
            start->platformShape.outCorner.pos, end->platformShape.inCorner.pos
        );

        ret.last = jumpPoint_generation_helper(midd_platform, end).last;        

    }
    else
    {
        // First jump was possible, return from recursion
        midd_platform->next = end;
        end->prev = midd_platform;//Set middle.next if end platform is close enogh        
        
        ret.last = midd_platform;
    }

    pl->moveto(start->platformShape.outCorner.pos);
    
    //Create jumppoint between start and new middle if jump not possible
    if(!this->pl->isJumpPossible(midd_platform->platformShape.inCorner.pos)){
        
        ret.first = jumpPoint_generation_helper(start, midd_platform).first;

    }
    else
    {
        start->next = midd_platform;//Set start.next if new middle platform is close enogh  
        midd_platform->prev = start;
        
        ret.first = midd_platform;
    }
    return ret;
}

void Position_generator::jumpPoint_create_offset(Platform* plat,vec3& currentMiddle, vec3 start, vec3 end)
{
    vec3 temp = vec3(randF(-1.f, 1.f), randF(-1.f, 1.f), randF(-1.f, 1.f)).Normalize();
    vec3 start_End = (end - start);
    vec3 start_End_dir = vec3::Normalize(start_End);
    float randomDist = randF(0.f, start_End.length()) / JP_conf.random_dist_dividier;

    while(  start_End_dir * temp > JP_conf.rand_dir_max_angle_percent || 
            start_End_dir * temp < JP_conf.rand_dir_min_angle_percent)
    {
        //to get all possible directions, use -1 and 1...
        temp = vec3(randF(-1.f, 1.f), randF(-1.f, 1.f), randF(-1.f, 1.f)).Normalize();
    }    
        
    vec3 randomDir = start_End_dir.X(temp).Normalize();
    randomDir.y = std::clamp(randomDir.y, JP_conf.y_min_clamp, JP_conf.y_max_clamp);    
    
    vec3 offset = randomDir * randomDist; 

    plat->setPosition(start + offset);
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
    
    this->startPlat = new Platform(vec3(0,AP_conf.spawn_Y_offset_origo,0), 0, 0);    
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

void Position_generator::setNrOfAnchors(int nrOfElements)
{    
    this->AP_conf.nrOfAnchors = nrOfElements;
}

void Position_generator::assignPlayer (Player_jump_checker* player) { this->pl = player; }

float Position_generator::randF (float min, float max)
{
    float random = min + ((float)rand ()) / ((float)RAND_MAX / (max - min));
    return random;
}

int Position_generator::getNrOfValidJumppoints()
{
    Platform* currentJumppoint = this->firstJumpPoint;
    int validMeshes = 0;
    while (currentJumppoint) {
        if (!currentJumppoint->platformShape.get_is_Illegal()) {
            validMeshes++;
        }
        currentJumppoint = currentJumppoint->next;
    }
    return validMeshes;
}

Platform* Position_generator::getFirstJumppoint()
{
    return this->firstJumpPoint;
}

Platform* Position_generator::getFirstAnchorpoint()
{
    return this->anchors[0];
}

int Position_generator::getNrOfValidAnchorpoints()
{
    int validMeshes = 0;    
    Platform* currentAnchor = this->getAnchors()->at(0);
    while (currentAnchor) {
        if (!currentAnchor->platformShape.get_is_Illegal()) {
            validMeshes++;
        }
        currentAnchor = currentAnchor->next;
    }
    return validMeshes;
}

mapDimensions Position_generator::getCurrentMapDimensions()
{
    mapDimensions currentMapDimension{
        0,
        0,
        vec2(0,0),
        vec2(0,0)
    }; 

    vec2 min; 
    vec2 max;

    std::vector<Platform*> allPlatforms;
    allPlatforms.insert(allPlatforms.begin(), this->anchors.begin(), this->anchors.end());
    allPlatforms.insert(allPlatforms.end(), this->jumpPoints.begin(), this->jumpPoints.end());

    //Get height    
    for (size_t i = 0; i < allPlatforms.size(); i++)
    {
        for(size_t j = 0; j < allPlatforms[i]->platformShape.previousVoxels.size(); j++){
            
            if(min.y > allPlatforms[i]->platformShape.previousVoxels[j].current_center.z){ //Get min Z 
                min.y = allPlatforms[i]->platformShape.previousVoxels[j].current_center.z - allPlatforms[i]->platformShape.get_scale().z;
            }

            if (min.x > allPlatforms[i]->platformShape.previousVoxels[j].current_center.x) { //Get min X 
                min.x = allPlatforms[i]->platformShape.previousVoxels[j].current_center.x - allPlatforms[i]->platformShape.get_scale().x;
            }
            if (max.y < allPlatforms[i]->platformShape.previousVoxels[j].current_center.z) { //Get min Z 
                max.y = allPlatforms[i]->platformShape.previousVoxels[j].current_center.z + allPlatforms[i]->platformShape.get_scale().z;
            }

            if (max.x < allPlatforms[i]->platformShape.previousVoxels[j].current_center.x) { //Get min X 
                max.x = allPlatforms[i]->platformShape.previousVoxels[j].current_center.x + allPlatforms[i]->platformShape.get_scale().x;
            }
        }
    }

    currentMapDimension.x_width = max.x - min.x;
    currentMapDimension.z_width = max.y - min.y;
    currentMapDimension.highPoint = max;
    currentMapDimension.lowPoint = min;

    return currentMapDimension;

}

void Position_generator::removeOverlappingPlatformVoxels()
{

    float platform_voxlel_marigin = Shape::shape_conf.plattform_voxel_margin;
    //Remove Jumppoints voxels that overlaps anchorpoints voxels
    for (int i = 0; i < this->anchors.size(); i++) {

        for (int A_voxel = 0; A_voxel < anchors[i]->platformShape.previousVoxels.size(); A_voxel++) {
            
            for (int j = 0; j < this->jumpPoints.size(); j++) {

                float length = (*anchors[i]->getPos()-*jumpPoints[j]->getPos()).length();
                
                float cullingDist = anchors[i]->platformShape.get_scale().length() * 2 * anchors[i]->platformShape.previousVoxels.size() +
                                    jumpPoints[j]->platformShape.get_scale().length() * 2 * jumpPoints[j]->platformShape.previousVoxels.size();

                for (int J_voxel = 0; J_voxel < jumpPoints[j]->platformShape.previousVoxels.size() && length < cullingDist; J_voxel++) {

                    vec3 diff = anchors[i]->platformShape.previousVoxels[A_voxel].current_center - jumpPoints[j]->platformShape.previousVoxels[J_voxel].current_center;
                    diff.x = std::fabsf(diff.x);
                    diff.y = std::fabsf(diff.y);
                    diff.z = std::fabsf(diff.z);

                    vec3 minCenterDist = anchors[i]->platformShape.get_scale() + jumpPoints[j]->platformShape.get_scale();

                    if(diff.x < minCenterDist.x && diff.z < minCenterDist.z &&  diff.y < minCenterDist.y + platform_voxlel_marigin){
                        //remove this jumppoint_voxel
                        auto iterator = jumpPoints[j]->platformShape.previousVoxels.begin() + J_voxel;
                        jumpPoints[j]->platformShape.previousVoxels.erase(iterator);
                        if(jumpPoints[j]->platformShape.previousVoxels.size() == 0){
                            jumpPoints[j]->platformShape.set_is_Illegal(true);
                        }
                        J_voxel--;
                    }
                }
            }
        }    
    }

    //Remove Jumppoints voxels that overlaps other Jumppoint voxels
    for (int i = 0; i < this->jumpPoints.size()-1; i++) {

        for (int A_voxel = 0; A_voxel < jumpPoints[i]->platformShape.previousVoxels.size(); A_voxel++) {

            for (int j = i+1; j < this->jumpPoints.size(); j++) {

                float length = (jumpPoints[i]->platformShape.get_midpoint() - jumpPoints[j]->platformShape.get_midpoint()).length();
                //jumpPoints[j]->platformShape.scale.length()*
               // float cullingDist = 4 * jumpPoints[j]->platformShape.previousVoxels.size(); 
                float cullingDist = jumpPoints[j]->platformShape.get_scale().length() * 2 * jumpPoints[j]->platformShape.previousVoxels.size() +
                                    jumpPoints[i]->platformShape.get_scale().length() * 2 * jumpPoints[i]->platformShape.previousVoxels.size();

                for (int J_voxel = 0; J_voxel < jumpPoints[j]->platformShape.previousVoxels.size() && length < cullingDist; J_voxel++) {

                    vec3 diff = jumpPoints[i]->platformShape.previousVoxels[A_voxel].current_center - jumpPoints[j]->platformShape.previousVoxels[J_voxel].current_center;
                    diff.x = std::fabsf(diff.x);
                    diff.y = std::fabsf(diff.y);
                    diff.z = std::fabsf(diff.z);

                    vec3 minCenterDist = jumpPoints[i]->platformShape.get_scale() + jumpPoints[j]->platformShape.get_scale();

                    if (diff.x < minCenterDist.x && diff.z < minCenterDist.z && diff.y < minCenterDist.y+platform_voxlel_marigin) {
                        //remove this jumppoint_voxel
                        auto iterator = jumpPoints[j]->platformShape.previousVoxels.begin() + J_voxel;
                        jumpPoints[j]->platformShape.previousVoxels.erase(iterator);
                        if (jumpPoints[j]->platformShape.previousVoxels.size() == 0) {
                            jumpPoints[j]->platformShape.set_is_Illegal(true);
                        }
                        J_voxel--;
                    }
                }
            }
        }
    }

    int breakME = 3;

}


Platform*& Position_generator::GetStartPlatform()
{
    return startPlat;
}

