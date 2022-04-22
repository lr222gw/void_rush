#include "Portal.h"

Portal::Portal(Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler) : gfx(gfx), rm(rm), colHandler(&colHandler)
{
}

Portal::~Portal()
{
    for (size_t i = 0; i < portals.size(); i++) {
        delete portals[i];
    }
    portals.clear();
}

void Portal::Spawn(vec3 pos)
{
    for (size_t i = 0; i < portals.size(); i++) 
    {
        colHandler->deletePlatform(portals[i]);
        delete portals[i];
    }
    portals.clear();
	this->spawned = true;

    portals.push_back(new GameObject(rm->get_Models("Portal.obj", gfx), gfx, vec3(pos.x -15.0f, pos.y + 9.0f, pos.z + 15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));
    portals.push_back(new GameObject(rm->get_Models("Portal.obj", gfx), gfx, vec3(pos.x, pos.y + 9.0f, pos.z + 15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));
    portals.push_back(new GameObject(rm->get_Models("Portal.obj", gfx), gfx, vec3(pos.x + 15.0f, pos.y + 9.0f, pos.z + 15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));
    for (size_t i = 0; i < portals.size(); i++)
    {
        colHandler->addPlatform(portals[i]);
    }
}

void Portal::InteractPortal(vec3 playerPos, vec3 forwardVec)
{
    float size, test;
    vec3 midPos;
    for (int i = 0; i < 3; i++)
    {
        midPos = GetMidPos(portals[i], size);
        if (CanInteract(playerPos, forwardVec, midPos, size / 2.0f, 5.0f, test))
        {
            if (i == 0)
            {
                std::cout << "Spawn next puzzle on easy mode" << std::endl;
                this->completed = true;
            }
            else if (i == 1)
            {
                std::cout << "Spawn next puzzle on medium mode" << std::endl;
                this->completed = true;
            }
            else
            {
                std::cout << "Spawn next puzzle on hard mode" << std::endl;
                this->completed = true;
            }
        }
    }
}

void Portal::ResetPortal()
{
    this->spawned = false;
    this->completed = false;
}

bool Portal::GetCompleted() const
{
    return this->completed;
}

bool Portal::GetStatePortal() const
{
	return this->spawned;
}

CollisionHandler* Portal::GetColHandlerBase()
{
    return this->colHandler;
}

void Portal::UpdatePortal()
{
    for (int i = 0; i < portals.size(); i++) {

        portals[i]->updateMatrix();
        portals[i]->update(0);
        portals[i]->updateVertexShader(gfx);
        portals[i]->updatePixelShader(gfx);
        portals[i]->draw(gfx);
    }
}
