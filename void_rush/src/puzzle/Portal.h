#pragma once

#include <random>
#include <string>
#include <iostream>
#include "3Dproj/GameObject.h"
#include "3Dproj/ResourceManager.h"
#include "3Dproj/Graphics.h"
#include <vector>
#include "interact/interact.hpp"
#include "3Dproj/CollisionHandler.h"

class Portal
{
private:
	std::vector<GameObject*> portals;
	bool spawned = false;
	bool completed = false;
	Graphics* gfx;
	ResourceManager* rm;

	CollisionHandler* colHandler;

public:
	Portal(Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler);
	virtual ~Portal();

	void Spawn(vec3 pos);

	void InteractPortal(vec3 playerPos, vec3 forwardVec, bool collided = false);

	void ResetPortal();

	bool GetCompleted() const;

	bool GetStatePortal() const;

	CollisionHandler* GetColHandlerBase();

	void UpdatePortal();
};

