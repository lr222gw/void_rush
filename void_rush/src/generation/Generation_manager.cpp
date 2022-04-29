#include "Generation_manager.hpp"

Generation_manager::Generation_manager(Graphics*& _gfx, ResourceManager*& _rm, CollisionHandler& collisionHandler)
	: gfx(_gfx), rm(_rm), seed((int)time(0)), difficulity(Difficulity::easy), player(nullptr), puzzleManager(nullptr), gameObjManager(nullptr)
{        
    
    this->position_gen = new Position_generator(this->seed);
    this->player_jump_checker = new Player_jump_checker();
    position_gen->assignPlayer(player_jump_checker);
    position_gen->setNrOfElements(20);
    this->collisionHandler = &collisionHandler;
}

Generation_manager::~Generation_manager()
{

    
    Platform* anchor = position_gen->getAnchors()->at(0);
    while(anchor){
        collisionHandler->deletePlatform(&anchor->platformShape);
        anchor = anchor->next;
    }
    Platform* jumppoint = position_gen->getJumpPoints()->at(0);
    while(jumppoint){
        collisionHandler->deletePlatform(&jumppoint->platformShape);
        jumppoint = jumppoint->next;
    }    
    gameObjManager->removeGameObject("map");
    
    /*for (int i = 0; i < this->nrOfAnchors; i++) {
        gameObjManager->removeGameObject("Anchor_" + std::to_string(i));
        collisionHandler->deletePlatform(platformObjs[i]);
    }
    for (int i = 0; i < this->nrOfJumpPoints; i++) {
        gameObjManager->removeGameObject("JumpPoint_" + std::to_string(i));
        collisionHandler->deletePlatform(platformObjs[i + this->nrOfAnchors]);
    }*/
    //for (PlatformObj* po : platformObjs) {
     //   delete po; 
    //}
    delete position_gen;
    delete player_jump_checker;    
}

void Generation_manager::set_player(Player* player)
{
    this->player = player;
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
        Platform* jumppoint_rm = position_gen->getJumpPoints()->at(0);
        while(jumppoint_rm){
            collisionHandler->deletePlatform(&jumppoint_rm->platformShape);
            jumppoint_rm = jumppoint_rm->next;
        }    

        gameObjManager->removeGameObject("map");
    }
    
    
    /*for (PlatformObj* po : platformObjs) {
        collisionHandler->deletePlatform(po);
    }*/
    platformObjs.clear();
    position_gen->reset_generation(this->player->getPos());

    position_gen->set_seed(this->seed);
    position_gen->start(difficulity);

    ///^vvvvvvvvvvvvvvvvvvvvvvv Testing
    
    
    shape_export.set_nrOf(position_gen->getAnchors()->size() + position_gen->getJumpPoints()->size(), 1); //TODO: do not hardcode!
    shape_export.init();

    Platform* anchor = position_gen->getAnchors()->at(0);
    while(anchor){
        anchor->platformShape.setShapeCube(*anchor->getPos());
        shape_export.build_shape_model(&anchor->platformShape, "map");
        collisionHandler->addPlatform(&anchor->platformShape);
        anchor = anchor->next;
    }
    Platform* jumppoint = position_gen->getJumpPoints()->at(0);
    while(jumppoint){
        jumppoint->platformShape.setShapeCube(*jumppoint->getPos());
        shape_export.build_shape_model(&jumppoint->platformShape, "map");
        collisionHandler->addPlatform(&jumppoint->platformShape);
        jumppoint = jumppoint->next;
    }

    //shape_export.export_final_model("map");
    platformObjs.push_back(
        new PlatformObj(rm->load_map_scene(shape_export.getScene(),"map", gfx),
        //new PlatformObj(rm->get_Models("map.obj",gfx),
            gfx,
            //vec3(anchor_pos->x + plane->offset.x, anchor_pos->y + plane->offset.y, anchor_pos->z + plane->offset.z),
            vec3(0.f,0.f,0.f),
            //plane->point1,
            vec3(0.f,0.f,0.f),
            
            vec3(1.0f, 1.0f, 1.0f))
    );
    this->nrOfAnchors++;
    gameObjManager->addGameObject(platformObjs[0], "map");

    ///^^^^^^^^^^^^^^^^^^^^^^^^^^ Testing

    //TODO: REMOVE THE COMMENTS!
    //place_anchorPoints();    
    //place_jumpPoints();

    puzzleManager->Initiate(this->getPuzzelPos());
    
}


void Generation_manager::place_anchorPoints()
{
    Platform* anchor_root = position_gen->getAnchors()->at(0);
    Platform* next_anchor = anchor_root;
    vec3* anchor_pos; 
    std::string identifier = "Anchor_";
    this->nrOfAnchors = 0;
    while (next_anchor) {
        anchor_pos = next_anchor->getPos();
        
        for(auto plane : next_anchor->platformShape.planes){
            vec3 copuu = plane->get_normal();
            platformObjs.push_back(
                new PlatformObj(rm->get_Models("plane.obj", gfx),
                    gfx,
                    //vec3(anchor_pos->x + plane->offset.x, anchor_pos->y + plane->offset.y, anchor_pos->z + plane->offset.z),
                    vec3(anchor_pos->x - plane->offset.z, anchor_pos->y + plane->offset.y, anchor_pos->z + plane->offset.x),
                    //plane->point1,
                    plane->get_rot(),
                    vec3(1.0f, 1.0f, 1.0f))
            );
        }
        
        
        /*
        for (auto plane : next_anchor->platformShape.planes) {
            platformObjs.push_back(
                new PlatformObj(rm->get_Models("plane.obj", gfx),
                    gfx,
                    plane->get_center(),
                    //vec3(anchor_pos->x, anchor_pos->y, anchor_pos->z),
                    normal_to_rot(normals.up, plane->get_normal()),
                    vec3(1.0f, 1.0f, 1.0f))
            );
        }
        
        */

        std::string temp = identifier + std::to_string(this->nrOfAnchors);
        gameObjManager->addGameObject(platformObjs[nrOfAnchors], temp);
        collisionHandler->addPlatform(platformObjs[nrOfAnchors]);
        next_anchor = next_anchor->next;
        this->nrOfAnchors++;
    }
    //this->nrOfAnchors--;
}

void Generation_manager::place_jumpPoints()
{
    Platform* jumpPoint_root = position_gen->getJumpPoints()->at(0);
    Platform* next_jumpPoint = jumpPoint_root;
    vec3* jumpPoint_pos;
    std::string identifier = "JumpPoint_";
    this->nrOfJumpPoints = 0;
    while (next_jumpPoint) {
        jumpPoint_pos = next_jumpPoint->getPos();
        for (auto plane : next_jumpPoint->platformShape.planes) {
            vec3 copuu = plane->get_normal();
            platformObjs.push_back(
                new PlatformObj(rm->get_Models("plane.obj", gfx),
                    gfx,
                    //vec3(anchor_pos->x + plane->offset.x, anchor_pos->y + plane->offset.y, anchor_pos->z + plane->offset.z),
                    vec3(jumpPoint_pos->x - plane->offset.z, 
                        jumpPoint_pos->y + plane->offset.y, 
                        jumpPoint_pos->z + plane->offset.x),
                    //plane->point1,
                    plane->get_rot(),
                    vec3(1.0f, 1.0f, 1.0f))
            );
        }

        /*
        for (auto plane : next_jumpPoint->platformShape.planes) {
            platformObjs.push_back(
                new PlatformObj(rm->get_Models("plane.obj", gfx),
                    gfx,
                    plane->get_center(),
                    // vec3(jumpPoint_pos->x, jumpPoint_pos->y, jumpPoint_pos->z),
                    normal_to_rot(normals.up, plane->get_normal()),
                    vec3(1.0f, 1.0f, 1.0f))
            );
        }
        */

        std::string temp = identifier + std::to_string(this->nrOfJumpPoints);
        gameObjManager->addGameObject(platformObjs[this->nrOfAnchors + this->nrOfJumpPoints],temp);
        collisionHandler->addPlatform(platformObjs[this->nrOfAnchors + this->nrOfJumpPoints]);
        next_jumpPoint = next_jumpPoint->next;
        this->nrOfJumpPoints++;
    }
    //this->nrOfJumpPoints--;
}

void Generation_manager::setDifficulty(Difficulity diff)
{
    this->difficulity = diff;
}

vec3 Generation_manager::getPuzzelPos()
{
    return *this->position_gen->getAnchors()->at(position_gen->getAnchors()->size()-1)->getPos();
}

void Generation_manager::draw()
{
    for (PlatformObj* platform : platformObjs) {

        //platform->updateMatrix();        
        //platform->update();
        platform->updateVertexShader(gfx);
        platform->updatePixelShader(gfx);
        platform->draw(gfx);            
    }
}

void Generation_manager::updatePlatfoms()
{
    //for (PlatformObj* platform : platformObjs) {
    //
    //    collisionWithBlocking(player, platform);
    //}
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
    int platforms_size = platforms->size();
    for (int i = 0; i < platforms_size; i++)
    {

        output_stream << i << " platform.\nXPos: " << platforms->at(i)->getPos()->x
            << " YPos: " << platforms->at(i)->getPos()->y
            << " ZPos: " << platforms->at(i)->getPos()->z << "\n"
            << "Rotation: " << platforms->at(i)->getRotation() << "\n";
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

PlatformObj::PlatformObj(ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale)
    : GameObject(file,gfx, pos,rot,scale)
{
}

PlatformObj::~PlatformObj()
{
}
