#include "Generation_manager.hpp"
#include "3Dproj/flags.h"

Generation_manager::Generation_manager(Graphics*& _gfx, ResourceManager*& _rm, CollisionHandler& collisionHandler, int seed)
	: gfx(_gfx), rm(_rm), seed(seed), difficulity(Difficulity::easy), player(nullptr), puzzleManager(nullptr), gameObjManager(nullptr)
{            
    this->shape_export = new Shape_exporter();
    this->position_gen = new Position_generator(this->seed);
    this->player_jump_checker = new Player_jump_checker();
    position_gen->assignPlayer(player_jump_checker);    
    
    this->collisionHandler = &collisionHandler;
    startSeed = -1;
    if (this->seed == -1) {
        this->seed = (int)time(0);
    }
}

Generation_manager::~Generation_manager()
{    
    Platform* anchor = position_gen->getAnchors()->at(0);
    while(anchor){
        collisionHandler->deletePlatform(&anchor->platformShape);
        anchor = anchor->next;
    }
    Platform* jumppoint = position_gen->firstJumpPoint;
    while(jumppoint){
        collisionHandler->deletePlatform(&jumppoint->platformShape);
        jumppoint = jumppoint->next;
    }    
    gameObjManager->removeGameObject("map_top");
    gameObjManager->removeGameObject("map_sides");
    gameObjManager->removeGameObject("map_bottom");
    gameObjManager->removeGameObject("map_topJP");
    gameObjManager->removeGameObject("map_sidesJP");
    gameObjManager->removeGameObject("map_bottomJP");
    
    delete player_jump_checker;    
    delete position_gen;
    delete shape_export;
}

void Generation_manager::set_player(Player* player)
{
    this->player = player;
    this->player_jump_checker->set_physics_params(
        player->getJumpForce(),
        player->getSpeed(), 
        player->getGravity());
}

void Generation_manager::set_PuzzleManager(ProtoPuzzle* puzzleManager)
{
    this->puzzleManager = puzzleManager;
}

void Generation_manager::set_GameObjManager(GameObjectManager* goMan)
{
    this->gameObjManager = goMan;
}

void Generation_manager::initialize()
{
    //Removes previous data and platforms if any
    if (position_gen->getAnchors()->size() > 0) {
        Platform* anchor_rm = position_gen->getAnchors()->at(0);
        while(anchor_rm){
            collisionHandler->deletePlatform(&anchor_rm->platformShape);
            anchor_rm = anchor_rm->next;
        }
        Platform* jumppoint_rm = position_gen->firstJumpPoint;
        while(jumppoint_rm){
            collisionHandler->deletePlatform(&jumppoint_rm->platformShape);
            jumppoint_rm = jumppoint_rm->next;
        }    

        gameObjManager->removeGameObject("map_top");
        gameObjManager->removeGameObject("map_sides");
        gameObjManager->removeGameObject("map_bottom");
        gameObjManager->removeGameObject("map_topJP");
        gameObjManager->removeGameObject("map_sidesJP");
        gameObjManager->removeGameObject("map_bottomJP");
    }
           
    platformObjs.clear();
    position_gen->reset_generation(this->player->getPos());
    
     
    
    this->player_jump_checker->set_physics_params(
        player->getJumpForce(),
        player->getSpeed(),
        //5.f,
        player->getGravity());

 
    if (startSeed == -1) {
        startSeed = seed;
        this->player->SetCurrentSeed(this->seed);
    }
    if (DEVMODE_ || DEBUGMODE) {
        position_gen->set_seed(this->seed);
    }
    else {
        position_gen->set_seed(this->seed++);
    }
    
    position_gen->start(difficulity);
    
    place_anchorPoints();  
    place_jumpPoints();  
           
    puzzleManager->Initiate(this->getPuzzelPos());  
    this->player->SetDifficulity(this->difficulity);
    this->player->SetStartPlatform(this->GetStartPlatform());
    
    /////////////////////SPIKE TEST/////////////////////
    gameObjManager->getGameObject("spikes")->setPos(this->getPuzzelPos() + vec3(0.0f, 5.01f, 0.0f));
    /////////////////////SNARE TEST/////////////////////
    gameObjManager->getGameObject("snare")->setPos(this->getPuzzelPos()+vec3(5.0f, 5.0f, 0.0f));
}


void Generation_manager::place_anchorPoints()
{
    Platform* anchor = position_gen->getFirstAnchorpoint();

    while (anchor) {
        if(!anchor->platformShape.get_is_Illegal()){
            anchor->platformShape.buildShape();        
            collisionHandler->addPlatform(&anchor->platformShape);            
        }
        anchor = anchor->next;
    }
    place_anchorPoints_top();
    place_anchorPoints_sides();
    place_anchorPoints_bottom();
}

void Generation_manager::place_anchorPoints_top()
{
    int nrOf_validMeshes = position_gen->getNrOfValidAnchorpoints();
    shape_export->set_nrOf(nrOf_validMeshes, 1); //TODO: do not hardcode material!
    shape_export->init(texturesEnum::top);

    Platform* anchor = position_gen->getFirstAnchorpoint();
    while (anchor) {
        if (!anchor->platformShape.get_is_Illegal()) {
            shape_export->build_shape_model(anchor->platformShape.top, "map_top");
        }
        anchor = anchor->next;
    }
    platformObjs.push_back(
        new PlatformObj(rm->load_map_scene(shape_export->getScene(), "map_top", gfx),
            gfx,
            vec3(0.f, 0.f, 0.f),
            vec3(0.f, 0.f, 0.f),
            vec3(1.0f, 1.0f, 1.0f))
    );
    gameObjManager->addGameObject(platformObjs[0], "map_top");
}
void Generation_manager::place_anchorPoints_bottom()
{
    int nrOf_validMeshes = position_gen->getNrOfValidAnchorpoints();
    shape_export->set_nrOf(nrOf_validMeshes, 1); //TODO: do not hardcode material!
    shape_export->init(texturesEnum::bottom);

    Platform* anchor = position_gen->getFirstAnchorpoint();

    while (anchor) {
        if (!anchor->platformShape.get_is_Illegal()) {
            shape_export->build_shape_model(anchor->platformShape.bottom, "map_bottom");
        }
        anchor = anchor->next;
    }
    platformObjs.push_back(
        new PlatformObj(rm->load_map_scene(shape_export->getScene(), "map_bottom", gfx),
            gfx,
            vec3(0.f, 0.f, 0.f),
            vec3(0.f, 0.f, 0.f),
            vec3(1.0f, 1.0f, 1.0f))
    );
    gameObjManager->addGameObject(platformObjs[2], "map_bottom");
}
void Generation_manager::place_anchorPoints_sides()
{
    int nrOf_validMeshes = position_gen->getNrOfValidAnchorpoints();
    shape_export->set_nrOf(nrOf_validMeshes, 1); //TODO: do not hardcode material!
    shape_export->init(texturesEnum::sides);

    Platform* anchor = position_gen->getFirstAnchorpoint();
    while (anchor) {
        if (!anchor->platformShape.get_is_Illegal()) {
            shape_export->build_shape_model(anchor->platformShape.sides, "map_sides");
        }
        anchor = anchor->next;
    }
    platformObjs.push_back(
        new PlatformObj(rm->load_map_scene(shape_export->getScene(), "map_sides", gfx),
            gfx,
            vec3(0.f, 0.f, 0.f),
            vec3(0.f, 0.f, 0.f),
            vec3(1.0f, 1.0f, 1.0f))
    );
    gameObjManager->addGameObject(platformObjs[1], "map_sides");
}

void Generation_manager::place_jumpPoints()
{
    Platform* jumppoint = position_gen->getFirstJumppoint();

    while (jumppoint) {

        if (!jumppoint->platformShape.get_is_Illegal()) {
            jumppoint->platformShape.buildShape();
            collisionHandler->addPlatform(&jumppoint->platformShape);
        }
        jumppoint = jumppoint->next;        
    }
    place_jumpPoints_top();
    place_jumpPoints_sides();
    place_jumpPoints_bottom();
}

void Generation_manager::place_jumpPoints_top()
{
    int nrOf_validMeshes = position_gen->getNrOfValidJumppoints();
    shape_export->set_nrOf(nrOf_validMeshes, 1); //TODO: do not hardcode material!
    shape_export->init(texturesEnum::top);

    Platform* jumppoint = position_gen->getFirstJumppoint();

    while (jumppoint) {

        if (!jumppoint->platformShape.get_is_Illegal()) {
            shape_export->build_shape_model(jumppoint->platformShape.top, "map_top");
        }
        jumppoint = jumppoint->next;

    }
    platformObjs.push_back(
        new PlatformObj(rm->load_map_scene(shape_export->getScene(), "map_topJP", gfx),
            gfx,
            vec3(0.f, 0.f, 0.f),
            vec3(0.f, 0.f, 0.f),
            vec3(1.0f, 1.0f, 1.0f))
    );
    gameObjManager->addGameObject(platformObjs[3], "map_topJP");
}
void Generation_manager::place_jumpPoints_bottom()
{
    int nrOf_validMeshes = position_gen->getNrOfValidJumppoints();
    shape_export->set_nrOf(nrOf_validMeshes, 1); //TODO: do not hardcode material!
    shape_export->init(texturesEnum::bottom);

    Platform* jumppoint = position_gen->getFirstJumppoint();
    while (jumppoint) {

        if (!jumppoint->platformShape.get_is_Illegal()) {
            shape_export->build_shape_model(jumppoint->platformShape.bottom, "map_bottom");
        }
        jumppoint = jumppoint->next;

    }
    platformObjs.push_back(
        new PlatformObj(rm->load_map_scene(shape_export->getScene(), "map_bottomJP", gfx),
            gfx,
            vec3(0.f, 0.f, 0.f),
            vec3(0.f, 0.f, 0.f),
            vec3(1.0f, 1.0f, 1.0f))
    );
    gameObjManager->addGameObject(platformObjs[5], "map_bottomJP");
}
void Generation_manager::place_jumpPoints_sides()
{
    int nrOf_validMeshes = position_gen->getNrOfValidJumppoints();
    shape_export->set_nrOf(nrOf_validMeshes, 1); //TODO: do not hardcode material!
    shape_export->init(texturesEnum::sides);
    Platform* jumppoint = position_gen->getFirstJumppoint();
    while (jumppoint) {

        if (!jumppoint->platformShape.get_is_Illegal()) {
            shape_export->build_shape_model(jumppoint->platformShape.sides, "map_sides");
        }
        jumppoint = jumppoint->next;

    }
    platformObjs.push_back(
        new PlatformObj(rm->load_map_scene(shape_export->getScene(), "map_sidesJP", gfx),
            gfx,
            vec3(0.f, 0.f, 0.f),
            vec3(0.f, 0.f, 0.f),
            vec3(1.0f, 1.0f, 1.0f))
    );
    gameObjManager->addGameObject(platformObjs[4], "map_sidesJP");
}

mapDimensions Generation_manager::getMapDimensions()
{    
    return this->position_gen->getCurrentMapDimensions();
}

void Generation_manager::setDifficulty(Difficulity diff)
{
    this->difficulity = diff;
}

Difficulity Generation_manager::getDifficulty() const
{
    return this->difficulity;
}

vec3 Generation_manager::getPuzzelPos()
{
    vec3 platformPosOffset = vec3(0.f, -20.f, 0.f);
    return *this->position_gen->getAnchors()->at(position_gen->getAnchors()->size()-1)->getPos() + platformPosOffset;
}

Platform*& Generation_manager::GetStartPlatform()
{
    return position_gen->GetStartPlatform();
}

void Generation_manager::draw()
{
    for (PlatformObj* platform : platformObjs) {
        
        platform->updateVertexShader(gfx);
        platform->updatePixelShader(gfx);
        platform->draw(gfx);            
    }
}

void Generation_manager::generateGraph()
{
    std::ofstream output_stream("void_rush/proto_outputs/debugoutput.txt");
    output_stream << "Platform Position_generator test\n Input seed: " << seed << std::endl;
    std::ofstream out("void_rush/proto_outputs/graph.dot");
    out << "digraph {\n";
    int scale = 20;

    float abs_X = 0;
    float abs_Y = 0;
    Player_jump_checker p;
    std::vector<Platform*>* platforms = this->position_gen->getAnchors();
    int platforms_size = (int)(platforms->size());
    for (int i = 0; i < platforms_size; i++)
    {

        output_stream << i << " platform.\nXPos: " << platforms->at(i)->getPos()->x
            << " YPos: " << platforms->at(i)->getPos()->y
            << " ZPos: " << platforms->at(i)->getPos()->z << "\n";            
        if (i != platforms->size() - 1)
        {
            output_stream << i << " Distance to next "
                << platforms->at(i)->distance(platforms->at(i)->next->getPos())
                << std::endl;
        }

        abs_X = platforms->at(i)->getPos()->y;
        abs_Y = platforms->at(i)->getPos()->x;

        out << "Platform_" << i << " [\n";
        out << "label = \"P_" << i << "\n";
        out << "X: " << platforms->at(i)->getPos()->x << "\n";
        out << "Y: " << platforms->at(i)->getPos()->y << "\n";
        out << "Z: " << platforms->at(i)->getPos()->z << "\"\n";

        out << "pos = \"" << abs_X * scale << "," << abs_Y * scale << "!\"\n";
        out << "]\n";
        if (i != 0)
        {
            out << "Platform_" << i - 1 << " -> "
                << "Platform_" << i;
            out << "[ label = \"Dist: "
                << platforms->at(i - 1)->distance(
                    platforms->at(i - 1)->next->getPos())
                << "\" ]"
                << "\n";
        }
    }
    out << "}\n";

    out.close();
    output_stream.close();
}

int Generation_manager::getStartSeed() const
{
    return startSeed;
}

PlatformObj::PlatformObj(ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale)
    : GameObject(file,gfx, pos,rot,scale)
{
}

PlatformObj::~PlatformObj()
{
}
