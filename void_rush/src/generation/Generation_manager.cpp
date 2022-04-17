#include "Generation_manager.h"

Generation_manager::Generation_manager(Graphics*& _gfx, ResourceManager*& _rm)
	: gfx(_gfx), rm(_rm)
{
}

Generation_manager::~Generation_manager()
{
    for (PlatformObj* po : platforms) {
        delete po; //TODO : causes memory leak?...
    }
}

void Generation_manager::initialize()
{
    platforms.push_back(
        new PlatformObj(rm->get_Models("platform.obj", gfx),
            gfx,
            vec3(5.0f, 5.0f, 0.0f),
            vec3(0.0f, 0.0f, 0.0f),
            vec3(2.05f, 3.3f, 4.05f))
    );

    
}

void Generation_manager::draw()
{
    for (PlatformObj* platform : platforms) {

        //platform->updateMatrix();        
        //platform->update();
        platform->updateVertexShader(gfx);
        platform->updatePixelShader(gfx);
        platform->draw(gfx);
    }
}

void Generation_manager::updatePlatfoms(Player* player)
{
    for (PlatformObj* platform : platforms) {

        collisionWithBlocking(player, platform);
    }
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
