#include "Generation_manager.hpp"

Generation_manager::Generation_manager(Graphics*& _gfx, ResourceManager*& _rm, CollisionHandler& collisionHandler)
	: gfx(_gfx), rm(_rm), seed((int)time(0)), difficulity(Difficulity::easy), player(nullptr), puzzleManager(nullptr), gameObjManager(nullptr)
{        
    
    this->position_gen = new Position_generator(this->seed);
    this->player_jump_checker = new Player_jump_checker();
    position_gen->assignPlayer(player_jump_checker);
    position_gen->setNrOfElements(29);
    this->collisionHandler = &collisionHandler;
}

Generation_manager::~Generation_manager()
{
    for (int i = 0; i < nrOfAnchors; i++) {
        gameObjManager->removeGameObject("Anchor " + std::to_string(i));
        collisionHandler->deletePlatform(platformObjs[i]);
    }
    for (int i = 0; i < nrOfJumpPoints; i++) {
        gameObjManager->removeGameObject("JumpPoint " + std::to_string(i));
        collisionHandler->deletePlatform(platformObjs[i + nrOfAnchors]);
    }
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
    for (int i = 0; i < nrOfAnchors; i++) {
        gameObjManager->removeGameObject("Anchor " + std::to_string(i));
        collisionHandler->deletePlatform(platformObjs[i]);
    }
    for (int i = 0; i < nrOfJumpPoints; i++) {
        gameObjManager->removeGameObject("JumpPoint " + std::to_string(i));
        collisionHandler->deletePlatform(platformObjs[i+nrOfAnchors]);
    }
    /*for (PlatformObj* po : platformObjs) {
        collisionHandler->deletePlatform(po);
    }*/
    platformObjs.clear();
    position_gen->reset_anchors(this->player->getPos());

    position_gen->set_seed(this->seed);
    position_gen->start(difficulity);

    place_anchorPoints();    
    place_jumpPoints();

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
        platformObjs.push_back(
            new PlatformObj(rm->get_Models("platform.obj", gfx),
                gfx,
                vec3(anchor_pos->x, anchor_pos->y, anchor_pos->z),
                vec3(0.0f, 0.0f, 0.0f),
                vec3(1.0f, 1.0f, 1.0f))
        );
        std::string temp = identifier + std::to_string(this->nrOfAnchors);
        gameObjManager->addGameObject(platformObjs[platformObjs.size() - 1], temp);
        collisionHandler->addPlatform(platformObjs[platformObjs.size() - 1]);
        next_anchor = next_anchor->next;
        this->nrOfAnchors++;
    }
    this->nrOfAnchors++;
}

void Generation_manager::place_jumpPoints()
{
    Platform* jumpPoint_root = position_gen->getJumpPoints()->at(0);
    Platform* next_jumpPoint = jumpPoint_root;
    vec3* jumpPoint_pos;
    std::string identifier = "JumpPoint ";
    this->nrOfJumpPoints = 0;
    while (next_jumpPoint) {
        jumpPoint_pos = next_jumpPoint->getPos();
        platformObjs.push_back(
            new PlatformObj(rm->get_Models("platform.obj", gfx),
                gfx,
                vec3(jumpPoint_pos->x, jumpPoint_pos->y, jumpPoint_pos->z),
                vec3(0.0f, 0.0f, 0.0f),
                vec3(0.5f, 0.5f, 0.5f))
        );

        gameObjManager->addGameObject(platformObjs[platformObjs.size() - 1], identifier +
            std::to_string(this->nrOfJumpPoints));
        collisionHandler->addPlatform(platformObjs[platformObjs.size() - 1]);
        next_jumpPoint = next_jumpPoint->next;
        this->nrOfJumpPoints++;
    }
    this->nrOfJumpPoints++;
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
