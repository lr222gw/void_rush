#include "Generation_manager.hpp"

Generation_manager::Generation_manager(Graphics*& _gfx, ResourceManager*& _rm)
	: gfx(_gfx), rm(_rm), position_gen(new Position_generator(time(0))), difficulity(Difficulity::easy)
{        
    Player_jump_checker* pl = new Player_jump_checker();
    position_gen->assignPlayer(pl);
    position_gen->setNrOfElements(29);
}

Generation_manager::~Generation_manager()
{
    for (PlatformObj* po : platformObjs) {
        delete po; //TODO : causes memory leak?...
    }
    delete position_gen;
}

void Generation_manager::initialize()
{

    position_gen->start(difficulity);
    /*
    for (int i = 0; i < position_gen->getPlatforms()->size(); i++) {
        platformObjs.push_back(
            new PlatformObj(rm->get_Models("platform.obj", gfx),
     
           gfx,
                vec3(5.0f, 5.0f, 0.0f),
                vec3(0.0f, 0.0f, 0.0f),
                vec3(2.05f, 3.3f, 4.05f))
        );
    }
    */
    Platform* platform_root = position_gen->getPlatforms()->at(0);    
    Platform* next_platform = platform_root;
    Vector3* platform_pos;
    while (next_platform) {
        platform_pos = next_platform->getPos();
        platformObjs.push_back(
            new PlatformObj(rm->get_Models("platform.obj", gfx),
                gfx,
                vec3(platform_pos->x, platform_pos->y, platform_pos->z),
                vec3(0.0f, 0.0f, 0.0f),
                vec3(2.05f, 3.3f, 4.05f))
        );
        next_platform = next_platform->next;
    }

    
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

void Generation_manager::updatePlatfoms(Player* player)
{
    for (PlatformObj* platform : platformObjs) {

        collisionWithBlocking(player, platform);
    }
}

void Generation_manager::generateGraph()
{
    std::ofstream output_stream("void_rush/proto_outputs/debugoutput.txt");
    output_stream << "Platform Position_generator test\n Input seed: " << seed << std::endl;
    std::ofstream out("void_rush/proto_outputs/graph.dot");
    out << "digraph {\n";
    int scale = 20;

    int abs_X = 0;
    int abs_Y = 0;
    Player_jump_checker p;
    std::vector<Platform*>* platforms = this->position_gen->getPlatforms();
    int platforms_size = platforms->size();
    for (int i = 0; i < platforms_size; i++)
    {
        //auto d = Vector3 (1.f, 2.f, 3.f);
        //p.distance (d);
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
        // abs_X += platforms->at(i)->getPos()->at(0) ;
        // abs_Y += platforms->at(i)->getPos()->at(1) ;

        abs_X = platforms->at(i)->getPos()->y;
        abs_Y = platforms->at(i)->getPos()->x;

        out << "Platform_" << i << " [\n";
        out << "label = \"P_" << i << "\n";
        out << "X: " << platforms->at(i)->getPos()->x << "\n";
        out << "Y: " << platforms->at(i)->getPos()->y << "\n";
        out << "Z: " << platforms->at(i)->getPos()->z << "\"\n";

        // out << "pos = \""<< platforms->at(i)->getPos()->at(0) * scale <<","
        // << platforms->at(i)->getPos()->at(1) * scale <<"!\"\n"; out << "pos =
        // \""<< abs_X
        // + scale <<"," << abs_Y + scale <<"!\"\n";

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
    //TODO: Dont know what this is.. is it needed?
    //GOPTR = static_cast<GameObject*>(this);
}

PlatformObj::~PlatformObj()
{
}
