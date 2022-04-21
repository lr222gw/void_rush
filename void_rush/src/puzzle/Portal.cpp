#include "Portal.h"

Portal::Portal(Graphics*& gfx, ResourceManager*& rm)
{
	portals.push_back(new GameObject(rm->get_Models("Portal.obj", gfx), gfx, vec3(-15.0f, 30.0f, 15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));
    portals.push_back(new GameObject(rm->get_Models("Portal.obj", gfx), gfx, vec3(0.0f, 30.0f, 15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));
    portals.push_back(new GameObject(rm->get_Models("Portal.obj", gfx), gfx, vec3(15.0f, 30.0f, 15.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));
}

Portal::~Portal()
{
    for (size_t i = 0; i < portals.size(); i++) {
        delete portals[i];
    }
}

void Portal::Spawn()
{
	this->spawned = true;

}

void Portal::InteractPortal(vec3 playerPos, vec3 forwardVec)
{
    float test;
    if (CanInteract(playerPos, forwardVec, portals[0]->getPos(), 5.0f, 5.0f, test))
    {
        std::cout << "Spawn next puzzle on easy mode" << std::endl;
    }
    if (CanInteract(playerPos, forwardVec, portals[1]->getPos(), 5.0f, 5.0f, test))
    {
        std::cout << "Spawn next puzzle on medium mode" << std::endl;
    }
    if (CanInteract(playerPos, forwardVec, portals[2]->getPos(), 5.0f, 5.0f, test))
    {
        std::cout << "Spawn next puzzle on hard mode" << std::endl;
    }

    /*
    if ((portals[0]->getPos() - playerPos).length() < 10.0f)
    {
        std::cout << "Spawn next puzzle on easy mode" << std::endl;
    }
    if ((portals[1]->getPos() - playerPos).length() < 10.0f)
    {
        std::cout << "Spawn next puzzle on medium mode" << std::endl;
    }
    if ((portals[2]->getPos() - playerPos).length() < 10.0f)
    {
        std::cout << "Spawn next puzzle on hard mode" << std::endl;
    }
    */
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
