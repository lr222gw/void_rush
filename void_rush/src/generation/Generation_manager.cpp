#include "Generation_manager.h"

Generation_manager::Generation_manager(Graphics*& _gfx, ResourceManager*& _rm)
	: gfx(_gfx), rm(_rm)
{
}

Generation_manager::~Generation_manager()
{
    for (PlatformObj* po : platforms) {
        //delete po; //TODO : causes memory leak?...
    }
}

void Generation_manager::initialize()
{
    /*platforms.push_back(
        new PlatformObj(rm->get_Models("platform.obj", gfx),
            gfx,
            vec3(-15.0f, 20.0f, 10.0f),
            vec3(0.0f, 0.0f, 0.0f),
            vec3(0.05f, 0.3f, 0.05f))
    );*/

    
}

void Generation_manager::update()
{
    for (PlatformObj* platform : platforms) {

        //platform.obj.updateMatrix();
        //platform.obj.update();
        //platform.obj.updateVertexShader(gfx);
        //platform.obj.updatePixelShader(gfx);
        //platform.obj.draw(gfx);
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
