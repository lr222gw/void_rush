#include "Portal.h"

Portal::Portal(Graphics*& gfx, ResourceManager*& rm, Generation_manager*& generationManager, CollisionHandler* collHandl)
    : gfx(gfx), rm(rm), generationManager(generationManager), collHandl(collHandl)
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
    for (size_t i = 0; i < portals.size(); i++) {
        delete portals[i];
    }
    portals.clear();
	this->spawned = true;

    portals.push_back(new GameObject(rm->get_Models("Portal.obj", gfx), gfx, vec3(pos.x -15.0f, pos.y + 9.0f, pos.z + 15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));
    portals.push_back(new GameObject(rm->get_Models("Portal.obj", gfx), gfx, vec3(pos.x, pos.y + 9.0f, pos.z + 15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));
    portals.push_back(new GameObject(rm->get_Models("Portal.obj", gfx), gfx, vec3(pos.x + 15.0f, pos.y + 9.0f, pos.z + 15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));

}

void Portal::InteractPortal(vec3 playerPos, vec3 forwardVec)
{

    float test;
    if (CanInteract(playerPos, forwardVec, portals[0]->getPos(), 5.0f, 5.0f, test))
    {
        std::cout << "Spawn next puzzle on easy mode" << std::endl;
        generationManager->initialize();
        ResetPortal();
    }
    if (CanInteract(playerPos, forwardVec, portals[1]->getPos(), 5.0f, 5.0f, test))
    {
        std::cout << "Spawn next puzzle on medium mode" << std::endl;
        generationManager->initialize();
        ResetPortal();
    }
    if (CanInteract(playerPos, forwardVec, portals[2]->getPos(), 5.0f, 5.0f, test))
    {
        std::cout << "Spawn next puzzle on hard mode" << std::endl;
        generationManager->initialize();
        ResetPortal();
    }
}

void Portal::ResetPortal()
{
    this->spawned = false;
}

bool Portal::GetStatePortal() const
{
	return this->spawned;
}

void Portal::UpdatePortal(Graphics*& gfx)
{
    for (int i = 0; i < portals.size(); i++) {

        portals[i]->updateMatrix();
        portals[i]->update(0);
        portals[i]->updateVertexShader(gfx);
        portals[i]->updatePixelShader(gfx);
        portals[i]->draw(gfx);
    }
}
