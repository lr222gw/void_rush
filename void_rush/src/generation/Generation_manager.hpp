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
#include "3Dproj/GameObjectManager.h"

#include "common/Helper.hpp"

#include "common/Shape_exporter.hpp"

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
	Generation_manager(Graphics*& gfx, ResourceManager*& rm, CollisionHandler& collisionHandler, int seed);
	~Generation_manager();
	void set_player(Player* player);
	void set_PuzzleManager(ProtoPuzzle* puzzleManager);
	void set_GameObjManager(GameObjectManager* goMan);
	void initialize();
	void place_anchorPoints();
	void place_jumpPoints();

	void setDifficulty(Difficulity diff);
	Difficulity getDifficulty()const;
	vec3 getPuzzelPos();
	Platform*& GetStartPlatform();

	void generateGraph();
	int getStartSeed()const;

	void draw();	
private:
	Player* player;
	ProtoPuzzle* puzzleManager;
	GameObjectManager* gameObjManager;
	Shape_exporter* shape_export; //TODO; temp
	friend class ImguiManager;
	int seed; 
	int startSeed;
	Player_jump_checker* player_jump_checker;
	Difficulity difficulity;
	Position_generator* position_gen;
	std::vector<PlatformObj*> platformObjs;
	Graphics*& gfx;
	ResourceManager*& rm;
	CollisionHandler* collisionHandler;
};