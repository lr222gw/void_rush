#pragma once

#include "../common/Vector.hpp"
#include <random>
#include <string>
#include <iostream>
#include "3Dproj/GameObject.h"
#include "3Dproj/ResourceManager.h"
#include "3Dproj/Graphics.h"
#include <vector>
#include "interact/interact.hpp"

#include "3Dproj/CollisionHandler.h"
#include "generation/Generation_manager.hpp"

class Portal
{
private:
	std::vector<GameObject*> portals;
	bool spawned = false;
	Graphics* gfx;
	ResourceManager* rm;

	Generation_manager*& generationManager;
	CollisionHandler* collHandl;

public:
	Portal(Graphics*& gfx, ResourceManager*& rm, Generation_manager*& generationManager, CollisionHandler* collHandl);
	virtual ~Portal();

	void Spawn(vec3 pos);

	void InteractPortal(vec3 playerPos, vec3 forwardVec);

	void ResetPortal();

	bool GetStatePortal() const;

	void UpdatePortal(Graphics*& gfx);
};

