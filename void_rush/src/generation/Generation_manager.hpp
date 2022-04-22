#pragma once
#include "3Dproj/ResourceManager.h"
#include "3Dproj/Graphics.h"
#include "3Dproj/GameObject.h"
#include "3Dproj/ResourceManager.h"
#include "3Dproj/Player.h"
#include "3Dproj/Collision3D.h"
#include "Position_generator.hpp"
#include <vector>
#include "3Dproj/CollisionHandler.h"
#include "puzzle/protoPuzzle.hpp"
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
	friend class ImguiManager;
	//GameObject* GOPTR;
};



class Generation_manager {
public:
	Generation_manager(Graphics*& gfx, ResourceManager*& rm, CollisionHandler& collisionHandler);
	~Generation_manager();
	void set_player(Player* player);
	void set_PuzzleManager(ProtoPuzzle* puzzleManager);
	void initialize();
	void generate();
	void place_anchorPoints();
	void place_jumpPoints();

	void setDifficulty(Difficulity diff);
	vec3 getPuzzelPos();

	void generateGraph();

	void draw();
	void updatePlatfoms();
private:
	Player* player;
	ProtoPuzzle* puzzleManager;
	friend class ImguiManager;
	int seed; 
	Player_jump_checker* player_jump_checker;
	Difficulity difficulity;
	Position_generator* position_gen;
	std::vector<PlatformObj*> platformObjs;
	Graphics*& gfx;
	ResourceManager*& rm;
	CollisionHandler* collisionHandler;
};