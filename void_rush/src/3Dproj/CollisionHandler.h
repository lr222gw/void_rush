#pragma once
#include "3Dproj/Collision3D.h"
#include "3Dproj/Player.h"
#include "enemies/Enemy.h"
#include "powerups/Powerups.h"
#include "3Dproj/GameObjectManager.h"
#include "common/Shape.hpp"

class Portal; 

struct portal_pair{
	Portal* portalManager;
	GameObject* portal;
};

class CollisionHandler {
public:
	CollisionHandler();
	void addPortal(portal_pair portal);
	void addPlatform(GameObject* platform);
	void addPlatform(Shape* platform);
	void addPlayer(Player* player);
	void addEnemies(Enemy* enemies);
	void addObstacle(GameObject* Obstacle);
	void addPearl(GameObject* pearl);
	void addPowerups(Powerups* powerups);
	void deletePlatform(GameObject* ptr);
	void deletePlatform(Shape* ptr);
	void deleteEnemies(Enemy* ptr);
	void deleteObstacle(GameObject* ptr);
	void deletePowerups(Powerups* ptr);
	void deletePortals(GameObject* ptr);
	void update();
private:

	std::vector<GameObject*> Custom_platforms;
	std::vector<Shape*> Generated_Platforms;
	std::vector<Enemy*> Enemies;
	std::vector<GameObject*> Obstacle;
	std::vector<Powerups*> powerups;
	std::vector<portal_pair> portals;
	GameObject* Pearl;
	Player* player;

private:
	friend class ImguiManager;
	Shape* lastCollided_ShapePlatform;
};