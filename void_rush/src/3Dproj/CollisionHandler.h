#pragma once
#include "3Dproj/Collision3D.h"
#include "3Dproj/Player.h"
#include "3Dproj/Enemy.h"
#include "3Dproj/Powerups.h"
#include "3Dproj/GameObjectManager.h"
#include "common/Shape.hpp"

class CollisionHandler {
public:
	CollisionHandler();
	void addPlatform(GameObject* platform);
	void addPlatform(Shape* platform);
	void addPlayer(Player* player);
	void addEnemies(Enemy* enemies);
	void addObstacle(GameObject* Obstacle);
	void addPowerups(Powerups* powerups);
	void deletePlatform(GameObject* ptr);
	void deletePlatform(Shape* ptr);
	void deleteEnemies(Enemy* ptr);
	void deleteObstacle(GameObject* ptr);
	void deletePowerups(Powerups* ptr);
	void update();
private:

	std::vector<GameObject*> Custom_platforms;
	std::vector<Shape*> Generated_Platforms;
	std::vector<Enemy*> Enemies;
	std::vector<GameObject*> Obstacle;
	std::vector<Powerups*> powerups;
	Player* player;


};