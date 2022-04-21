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

class Portal
{
private:
	std::vector<GameObject*> portals;
	bool spawned = false;
	Graphics* gfx;
	ResourceManager* rm;
public:
	Portal(Graphics*& gfx, ResourceManager*& rm);
	virtual ~Portal();

	void Spawn();

	void InteractPortal(vec3 playerPos, vec3 forwardVec);

	void ResetPortal();

	bool GetStatePortal() const;

	void UpdatePortal(Graphics*& gfx);
};

