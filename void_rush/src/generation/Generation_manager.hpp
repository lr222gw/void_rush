#pragma once
#include "3Dproj/ResourceManager.h"
#include "3Dproj/Graphics.h"
#include "3Dproj/GameObject.h"
#include "3Dproj/ResourceManager.h"
#include "3Dproj/Player.h"
#include "3Dproj/Collision3D.h"
#include "Position_generator.hpp"
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

	void generateGraph();

	void draw();
	void updatePlatfoms(Player* player);
private:
	friend class ImguiManager;
	int seed; 
	Player_jump_checker* player_jump_checker;
	Difficulity difficulity;
	Position_generator* position_gen;
	std::vector<PlatformObj*> platformObjs;
	Graphics*& gfx;
	ResourceManager*& rm;
};