#include <iostream>
#include "Position_generator.hpp"
#include <algorithm>

Position_generator::PowerUp_position_settings Position_generator::PU_conf;
Position_generator::enemy_Position_settings Position_generator::enemyPos_conf;
std::vector<Platform*> Position_generator::getAllPlatforms()
{
    
    std::vector<Platform*> allPlatforms;
    allPlatforms.insert(allPlatforms.begin(), this->anchors.begin(), this->anchors.end());
    allPlatforms.insert(allPlatforms.end(), this->jumpPoints.begin(), this->jumpPoints.end());

    return allPlatforms;
}
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

    replace_random_jumpPoints_with_obstacles_positions();

    //removeOverlappingPlatformVoxels();
    removeUnnecessaryPlatforms();

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
            newPlat->platformShape.setAnchorShape(*newPlat->getPos(), dVect.length(), &current->platformShape);

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

void Position_generator::replace_random_jumpPoints_with_obstacles_positions()
{
    this->obstaclePositions.nrOfPositions = 0;
    this->obstaclePositions.positions.clear();

    std::vector<Platform*> replaceable_jumpPoints;

    //Find all JPs with a size of max 4 voxels
    for(auto& jp : this->getInOrderVector_ValidJumppoints()){

        if(jp->platformShape.previousVoxels.size() < 5){
            if(jp->prev &&
                 jp->next &&
                (jp->next->platformShape.get_midpoint() - jp->platformShape.get_midpoint()).length() > 3 && 
                (jp->prev->platformShape.get_midpoint() - jp->platformShape.get_midpoint()).length() > 3)
            {
                replaceable_jumpPoints.push_back(jp);
            }else{
                int breakMe = 3;
            }
        }
    }

    for (auto& ap : this->getInOrderVector_ValidAnchors()) {

        for (int i = 0; i < replaceable_jumpPoints.size(); i++) {

            auto rp = replaceable_jumpPoints[i];
            if (rp->prev &&
                rp->next &&
                ap->prev &&
                ap->next &&
                (ap->next->platformShape.get_midpoint() - rp->platformShape.get_midpoint()).length() < 3 &&
                (ap->prev->platformShape.get_midpoint() - rp->platformShape.get_midpoint()).length() < 3)
            {
                replaceable_jumpPoints.erase(replaceable_jumpPoints.begin() + i);
                
            }
            else {
                int breakMe = 3;
            }
        }                
    }

    //try to look for this nr of JPs to replace
    int nrOfObstacles = replaceable_jumpPoints.size() /2 ;
    
    for (int i = 0; i < nrOfObstacles; i++) {
        int pick_index = rand() % replaceable_jumpPoints.size();
        Platform* pick = replaceable_jumpPoints[pick_index];

        obstaclePositions.positions.push_back(pick->platformShape.get_midpoint());
        obstaclePositions.nrOfPositions++; 

        pick->platformShape.remove_all_planes();
        pick->platformShape.set_is_Illegal(true);

        replaceable_jumpPoints.erase(replaceable_jumpPoints.begin() + pick_index);        
    }


}

void Position_generator::select_powerUp_positions()
{
    this->powerup_positions.nrOfPositions = 0;
    this->powerup_positions.positions.clear();
    std::vector<Platform*> validJumpPoints = getInOrderVector_ValidJumppoints();

    for (int i = 0; i < validJumpPoints.size(); i+=PU_conf.powerUp_occurance_rate) {
                    
        int pickVoxelPos = rand() % validJumpPoints[i]->platformShape.previousVoxels.size();
        this->powerup_positions.nrOfPositions++;
        this->powerup_positions.positions.push_back(
            validJumpPoints[i]->platformShape.previousVoxels[pickVoxelPos].current_center + PU_conf.position_offset
        );
        
    }     
}

void Position_generator::select_enemy_positions()
{
    this->enemy_positions.nrOfPositions = 0;
    this->enemy_positions.positions.clear();
    std::vector<Platform*> validAnchors = this->getInOrderVector_ValidAnchors();
    this->enemy_positions.positions.resize(validAnchors.size());

    //Note: we skip the first anchor since it's the startplatform...
    //Random Positions
    for (int i = 1; i < validAnchors.size(); i++) { 
        std::vector <vec3> EnemyPositions;
        std::vector <int> busyIndex;
        for (int b = 0; b < validAnchors[i]->platformShape.previousVoxels.size(); b++) { busyIndex.push_back(b); }
        int nrOfEnemyPositions = rand() % validAnchors[i]->platformShape.previousVoxels.size();
        nrOfEnemyPositions = std::clamp(
            nrOfEnemyPositions,
            1,
            static_cast<int>(validAnchors[i]->platformShape.previousVoxels.size() / 1.65f));

        for (int j = 0; j < nrOfEnemyPositions; j++) {

            int pickVoxelPos = rand() % busyIndex.size();
            
            int poses_x = (validAnchors[i]->platformShape.get_scale().x / Shape::shape_conf.default_scale.x) ; 
            int poses_z = (validAnchors[i]->platformShape.get_scale().z / Shape::shape_conf.default_scale.z) ; 

            vec3 final_random_pos =
                validAnchors[i]->platformShape.previousVoxels[busyIndex[pickVoxelPos]].current_center;
            final_random_pos.x += static_cast<float>( rand() % poses_x) * Shape::shape_conf.default_scale.x;
            final_random_pos.z += static_cast<float>( rand() % poses_z) * Shape::shape_conf.default_scale.z;

            this->enemy_positions.nrOfPositions++;
            
            EnemyPositions.push_back(final_random_pos + enemyPos_conf.enemy_offset);
            busyIndex.erase(busyIndex.begin() + pickVoxelPos);
            
        }
        this->enemy_positions.positions[i].ranmdomPositions = EnemyPositions;
        
    }

    //Outside Positions
    for (int i = 1; i < validAnchors.size(); i++) {
        std::vector <vec3> EnemyPositions;
        std::vector <vec3> positionsAlongZ;
        
        int nrOf = validAnchors[i]->platformShape.previousVoxels.size();
        
        for(auto p : validAnchors[i]->platformShape.previousVoxels){
            
            bool wasFound = false;
            for(auto& v : positionsAlongZ){

                if (p.current_center.x > v.x) {
                    v.x = p.current_center.x;
                }
                if (p.current_center.z == v.z) {                    
                    wasFound = true;
                    break;
                }
            }
            if (!wasFound) { positionsAlongZ.push_back(p.current_center); }
        }

        for (int j = 0; j < positionsAlongZ.size(); j++) {
            
            positionsAlongZ[j].x += 
                Shape::shape_conf.default_scale.x * 
                Shape::shape_conf.scaleAnchor_XY_Factor * 
                enemyPos_conf.outsideOffset;
            
                
            this->enemy_positions.nrOfPositions++;

            EnemyPositions.push_back(positionsAlongZ[j] + enemyPos_conf.enemy_offset);
        }
        this->enemy_positions.positions[i].outsidePositions = EnemyPositions;
    }    
    struct vector_z_pair{
        float z;
        std::vector <vec3 > alongX_per_Z;
    };
    //Pattern Positions
    for (int i = 1; i < validAnchors.size(); i++) {
        std::vector <vec3> EnemyPositions;

        std::vector<vector_z_pair> positionsAlongZ;
        
        int nrOf = validAnchors[i]->platformShape.previousVoxels.size();

        for (auto& p : validAnchors[i]->platformShape.previousVoxels) {

            bool wasFound = false;
            for (auto& v : positionsAlongZ) {

                if (p.current_center.z == v.z) {
                    wasFound = true;                    
                    break;
                }
            }
            if (!wasFound) { positionsAlongZ.push_back(vector_z_pair{p.current_center.z}); }
        }

        for (auto& p : validAnchors[i]->platformShape.previousVoxels) {

            bool wasFound = false;
            for (auto& v : positionsAlongZ) {

                if (p.current_center.z == v.z) {
                    v.alongX_per_Z.push_back(p.current_center);
                }
            }           
        }

        for (int j = 0; j < positionsAlongZ.size(); j += 2) {

            for(auto &row : positionsAlongZ[j].alongX_per_Z){

                this->enemy_positions.nrOfPositions++;

                EnemyPositions.push_back(row + enemyPos_conf.enemy_offset);
            }
            
        }
        this->enemy_positions.positions[i].patternPositions = EnemyPositions;
    }

}

powerUp_positions* Position_generator::get_powerUp_positions()
{
    return &powerup_positions;
}

Enemy_positions* Position_generator::get_enemy_positions()
{
    return &enemy_positions;
}

ShortCut_positions* Position_generator::get_shortcut_positions()
{
    return &shortcut_positions;
}

ShortCut_positions* Position_generator::get_obstacle_positions()
{
    return &this->obstaclePositions;
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
    outCorner* startOutCorner = &start->platformShape.outCorner;
    inCorner* middInCorner = &midd_platform->platformShape.inCorner;
    outCorner* middOutCorner = &midd_platform->platformShape.outCorner;
    inCorner* endInCorner = &end->platformShape.inCorner;
    float prev_StartMidd_Dist = (startOutCorner->pos - middInCorner->pos).length();
    float prev_MiddEnd_Dist = (middOutCorner->pos - endInCorner->pos).length();
    float startMidd_dist = -1;
    float middEnd_dist = -1;
    for(int i = 0; i < middOutCorner->points.size(); i++){
        for(int j = 0; j < endInCorner->points.size(); j++){
            middEnd_dist = (middOutCorner->points[i] - endInCorner->points[j]).length();
            startMidd_dist = (startOutCorner->points[i] - middInCorner->points[j]).length();
            if(middEnd_dist < prev_MiddEnd_Dist){
                middOutCorner->pos = middOutCorner->points[i];
                endInCorner->pos = endInCorner->points[j];
                prev_MiddEnd_Dist = middEnd_dist;
            }
            if(startMidd_dist < prev_StartMidd_Dist){
                startOutCorner->pos = startOutCorner->points[i];
                middInCorner->pos = middInCorner->points[j];
                prev_StartMidd_Dist = startMidd_dist;
            }
        }
    }

    pl->moveto(midd_platform->platformShape.outCorner.pos);



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
    vec3 offset = create_offset(end, start);

    plat->setPosition(start + offset);
}

vec3 Position_generator::create_offset(vec3& end, const vec3& start)
{
    vec3 temp = vec3(randF(-1.f, 1.f), randF(-1.f, 1.f), randF(-1.f, 1.f)).Normalize();
    vec3 start_End = (end - start);
    vec3 start_End_dir = vec3::Normalize(start_End);
    float randomDist = randF(0.f, start_End.length()) / JP_conf.random_dist_dividier;

    while (start_End_dir * temp > JP_conf.rand_dir_max_angle_percent ||
        start_End_dir * temp < JP_conf.rand_dir_min_angle_percent)
    {
        //to get all possible directions, use -1 and 1...
        temp = vec3(randF(-1.f, 1.f), randF(-1.f, 1.f), randF(-1.f, 1.f)).Normalize();
    }

    vec3 randomDir = start_End_dir.X(temp).Normalize();
    randomDir.y = std::clamp(randomDir.y, JP_conf.y_min_clamp, JP_conf.y_max_clamp);

    return randomDir * randomDist; 
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
std::vector<Platform*> Position_generator::getInOrderVector_ValidJumppoints()
{
    Platform* currentJumppoint = this->firstJumpPoint;
    std::vector<Platform*> validJPs;
    while (currentJumppoint) {
        if (!currentJumppoint->platformShape.get_is_Illegal()) {
            validJPs.push_back(currentJumppoint);
        }
        currentJumppoint = currentJumppoint->next;
    }
    //std::reverse(validJPs.begin(), validJPs.end());
    return validJPs;
}
std::vector<Platform*> Position_generator::getInOrderVector_ValidAnchors()
{
    Platform* currentAnchor = this->anchors.front();
    std::vector<Platform*> validAPs;
    while (currentAnchor) {
        if (!currentAnchor->platformShape.get_is_Illegal()) {
            validAPs.push_back(currentAnchor);
        }
        currentAnchor = currentAnchor->next;
    }
    //std::reverse(validAPs.begin(), validAPs.end());
    return validAPs;
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

void Position_generator::generate_shortcut()
{
    shortcut_positions.nrOfPositions = 0; 
    shortcut_positions.positions.clear();

    std::vector<Platform*> valid_anchors = getInOrderVector_ValidAnchors();

    int randomAnchor = rand() % valid_anchors.size();
    int nextRandomAnchor = randomAnchor + 2;

    if(nextRandomAnchor >= (int)valid_anchors.size()) {
        randomAnchor -= 2;
        nextRandomAnchor -= 2;
    }

    //shortcut_positions.positions.push_back(valid_anchors[randomAnchor]->platformShape.outCorner.pos);
    
    Player_jump_checker mushroomJump;
    
    mushroomJump.set_physics_params(8.f,5.f, -15.f);//TODO: do not hardcode
    
    vec3 direction = 
        valid_anchors[nextRandomAnchor]->platformShape.inCorner.pos - 
        valid_anchors[randomAnchor]->platformShape.outCorner.pos;

    vec3 dir_unitvec = vec3::Normalize(direction);

    vec3 controlDir = 
        valid_anchors[nextRandomAnchor - 1]->platformShape.inCorner.pos - 
        valid_anchors[randomAnchor]->platformShape.outCorner.pos;

    

    vec3 normalized_controlDir = vec3::Normalize(controlDir);
    float dot_angle =  dir_unitvec * normalized_controlDir ;

    int triedCounter = 0;
    
    while (dot_angle < -this->JP_conf.minimumShortcutDotAngle || dot_angle > this->JP_conf.minimumShortcutDotAngle  
        && triedCounter <= valid_anchors.size()) 
    {

        randomAnchor     = (++randomAnchor) % (valid_anchors.size() - 2);
        nextRandomAnchor =  randomAnchor + 2;

        controlDir = 
            valid_anchors[nextRandomAnchor - 1]->platformShape.inCorner.pos - 
            valid_anchors[randomAnchor]->platformShape.outCorner.pos;

        direction = 
            valid_anchors[nextRandomAnchor]->platformShape.inCorner.pos - 
            valid_anchors[randomAnchor]->platformShape.outCorner.pos;

        dir_unitvec = vec3::Normalize(direction);

        normalized_controlDir = vec3::Normalize(controlDir);
        dot_angle =  dir_unitvec * normalized_controlDir ;
        triedCounter++;
    }

    if (triedCounter <= valid_anchors.size()) {

        vec3 originalDir = direction;
        mushroomJump.moveto(valid_anchors[randomAnchor]->platformShape.outCorner.pos);
        float start_current_len = 0; 
        float jumplen = mushroomJump.getJumpDistance();

        bool jumpPossible = !mushroomJump.isJumpPossible(valid_anchors[nextRandomAnchor]->platformShape.inCorner.pos);
        while (jumpPossible &&
            originalDir.length() - jumplen > start_current_len)
        {

            direction = valid_anchors[nextRandomAnchor]->platformShape.inCorner.pos - mushroomJump.getPos();
            dir_unitvec = vec3::Normalize(direction);

            //calc position for next platform
            vec3 offset = dir_unitvec * mushroomJump.getJumpDistance();
            vec3 newPos = mushroomJump.getPos() + offset;

            //get and set an random offset for newPos 
            auto temp = newPos + offset;
            vec3 v = create_offset(temp, mushroomJump.getPos()) / 2.f;
            v.y = 0.f;
            newPos = newPos + v;

            

            vec3 currentPlayerPos = valid_anchors[randomAnchor]->platformShape.outCorner.pos;
            
            this->shortcut_positions.positions.push_back(newPos);
            currentPlayerPos = this->shortcut_positions.positions.back();
            mushroomJump.moveto(currentPlayerPos);
            start_current_len = (newPos - valid_anchors[randomAnchor]->platformShape.outCorner.pos).length();

            jumpPossible = !mushroomJump.isJumpPossible(valid_anchors[nextRandomAnchor]->platformShape.inCorner.pos);
            

        }

        for (int i = 0; i <  this->shortcut_positions.positions.size(); i++)
        {

            //Check that position is not above/below an existing platform
            bool notAbove = check_platform_y_intersection(this->shortcut_positions.positions[i]);

            if(!notAbove){
                this->shortcut_positions.positions.erase(
                    this->shortcut_positions.positions.begin() + i
                );
            }
        }
    }
}

bool Position_generator::check_platform_y_intersection(const vec3& newPos)
{
    bool notAbove = true;
    float radius = 5;
    for (auto& p : this->getAllPlatforms()) {
        for (auto& v : p->platformShape.previousVoxels) {
            if ((v.current_center - newPos).length_XZ() < radius) {
                notAbove = false;
                break;
            }
        }
        if (!notAbove) {
            break;
        }
    }
    return notAbove;
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

    std::vector<Platform*> allPlatforms = getAllPlatforms();

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
                //jumpPoints[j]->platformShape.update_InOut(&jumpPoints[j]->prev->platformShape);
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
                jumpPoints[j]->platformShape.update_InOut(&jumpPoints[j]->prev->platformShape);
            }
        }
    }

    int breakME = 3;

}

void Position_generator::removeUnnecessaryPlatforms()
{
    int maxNrAhead = 5;
    bool skipable = false; 
    int iteration = 0;

    //Platform* startPoint = this->getFirstJumppoint();
    //Platform* toCheck = nullptr;

    //toCheck = startPoint->next;
    //while(toCheck && startPoint && toCheck->next && !startPoint->platformShape.get_is_Illegal()){
    //    iteration = 0;
    //    toCheck = startPoint->next;

    //    while (iteration < maxNrAhead && toCheck->next) {

    //        /*for (int j = 0; j < iteration && toCheck->next; j++) {
    //            toCheck = toCheck->next;
    //        }*/

    //        this->pl->moveto(startPoint->platformShape.outCorner.pos);
    //        float JumpDist = this->pl->getJumpDistance(toCheck->platformShape.inCorner.pos.y);
    //        float distance = this->pl->distance(toCheck->platformShape.inCorner.pos);
    //        float padding = JumpDist/4.f;
    //        skipable = this->pl->isJumpPossible(toCheck->platformShape.inCorner.pos);

    //        if (skipable && JumpDist > distance+padding) {
    //            toCheck->platformShape.previousVoxels.clear();
    //            toCheck->platformShape.set_is_Illegal(true);
    //        }
    //        else{
    //            startPoint = toCheck;
    //            break;
    //        }
    //        iteration++;
    //        toCheck = toCheck->next;
    //    }
    //    startPoint = toCheck;
    //}

    std::vector<Platform*> inOrderJumpPoints = this->getInOrderVector_ValidJumppoints();
    float jumpDist = 0;
    float distance = 0;
    float padding = 0;
    int j = 0;
    this->pl->moveto(this->anchors[0]->platformShape.outCorner.pos);
    for (int i = 1; i < this->anchors.size(); i++) {
        iteration = 0;
        for (j;j < inOrderJumpPoints.size(); j++) {
            jumpDist = this->pl->getJumpDistance(anchors[i]->platformShape.inCorner.pos.y);
            distance = this->pl->distance(anchors[i]->platformShape.inCorner.pos);
            //padding = jumpDist / jumpDist;
            padding = jumpDist/10.0f;
            if(jumpDist > distance+padding && this->pl->isJumpPossible(anchors[i]->platformShape.inCorner.pos)){
                inOrderJumpPoints[j]->next->platformShape.previousVoxels.clear();
                inOrderJumpPoints[j]->next->platformShape.set_is_Illegal(true);
                this->pl->moveto(this->anchors[i]->platformShape.outCorner.pos);
                break;
            }
            else {
                iteration = 0;
                while (iteration < maxNrAhead && inOrderJumpPoints[j + iteration]->next && j+iteration < inOrderJumpPoints.size()) {
                    jumpDist = this->pl->getJumpDistance(inOrderJumpPoints[j+iteration]->platformShape.inCorner.pos.y);
                    distance = this->pl->distance(inOrderJumpPoints[j+iteration]->platformShape.inCorner.pos);
                    padding = jumpDist / 10.f;
                    skipable = this->pl->isJumpPossible(inOrderJumpPoints[j+iteration]->platformShape.inCorner.pos);
                    if (skipable && jumpDist > distance + padding) {
                        inOrderJumpPoints[j+iteration]->platformShape.previousVoxels.clear();
                        inOrderJumpPoints[j+iteration]->platformShape.set_is_Illegal(true);
                        iteration++;
                    }
                    else {
                        j += iteration;
                        this->pl->moveto(inOrderJumpPoints[j]->platformShape.outCorner.pos);
                        break;
                    }
                }
            }
        }

    }
}


Platform*& Position_generator::GetStartPlatform()
{
    return startPlat;
}

