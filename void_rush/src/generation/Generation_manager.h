#pragma once
#include "3Dproj/ResourceManager.h"
#include "3Dproj/Graphics.h"
#include "3Dproj/GameObject.h"
#include "3Dproj/ResourceManager.h"
#include "Platform.hpp"
#include <vector>

//struct Platform{
//	GameObject obj;
//	Platform info;	
//};

class PlatformObj : public GameObject 
{
public:
	PlatformObj(ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale);
	~PlatformObj();
private:
	//GameObject* GOPTR;
};

class Generation_manager {
public:
	Generation_manager(Graphics*& gfx, ResourceManager*& rm);
	~Generation_manager();
	void initialize();
	void update();
private:

	std::vector<PlatformObj*> platforms;
	Graphics*& gfx;
	ResourceManager*& rm;
};