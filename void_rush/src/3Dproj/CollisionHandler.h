#pragma once
#include "3Dproj/Collision3D.h"
#include "3Dproj/Player.h"
#include "3Dproj/GameObjectManager.h"

class CollisionHandler {
public:
	CollisionHandler();
	void addPlatform(GameObject* platform);
	void addPlayer(Player* player);
	void addEnemies(GameObject* enemies);
	void addObstacle(GameObject* Obstacle);
	void deletePlatform(GameObject* ptr);
	void deleteEnemies(GameObject* ptr);
	void deleteObstacle(GameObject* ptr);
	void update();
private:

	std::vector<GameObject*> Platforms;
	std::vector<GameObject*> Enemies;
	std::vector<GameObject*> Obstacle;
	Player* player;


};