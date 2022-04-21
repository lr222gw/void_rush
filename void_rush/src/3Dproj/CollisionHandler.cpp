#include "CollisionHandler.h"

CollisionHandler::CollisionHandler()
{
}

void CollisionHandler::addPlatform(GameObject* platform)
{
	this->Platforms.push_back(platform);
}

void CollisionHandler::addPlayer(Player* player)
{
	this->player = player;
}

void CollisionHandler::addEnemies(Enemy* enemies)
{
	this->Enemies.push_back(enemies);
}

void CollisionHandler::addObstacle(GameObject* Obstacle)
{
	this->Obstacle.push_back(Obstacle);
}

void CollisionHandler::deletePlatform(GameObject* ptr)
{
	for (size_t i = 0; i < Platforms.size(); i++) {
		if (Platforms[i] == ptr) {
			Platforms.erase(std::next(Platforms.begin(), i));
		}
	}
}

void CollisionHandler::deleteEnemies(Enemy* ptr)
{
	for (size_t i = 0; i < Enemies.size(); i++) {
		if (Enemies[i] == ptr) {
			Enemies.erase(std::next(Enemies.begin(), i));
		}
	}
}

void CollisionHandler::deleteObstacle(GameObject* ptr)
{
	for (size_t i = 0; i < Obstacle.size(); i++) {
		if (Obstacle[i] == ptr) {
			Obstacle.erase(std::next(Obstacle.begin(), i));
		}
	}
}

void CollisionHandler::update()
{
	//check player with platforms
	bool done = false;
	for (size_t i = 0; i < Platforms.size(); i++) {
		if (!done && collision3D(player->getPlayerObjPointer(), Platforms[i], true, false))
		{
			done = true;

			if (player->getGroundedTimer() > 0.5f)
			{
				player->setGrounded();
			}
		}
		collisionWithBlocking(Platforms[i], player);
	}


	//check player with enemies // different things happens based on what enemy it is
	for (size_t i = 0; i < Enemies.size(); i++) {
		if (collision3D(player->getPlayerObjPointer(), Enemies[i])) {
			Enemies[i]->collidedWithPlayer();
		}
	}
	
	//check player with Obstacle // different things happens based on what Obstacle it is
	for (size_t i = 0; i < Obstacle.size(); i++) {

	}

	//check enemies with platform
	for (size_t i = 0; i < Enemies.size(); i++) {
		for (size_t o = i; o < Platforms.size(); o++) {

		}
	}

	//check enemies with Obstacle // don't know what happens here
	for (size_t i = 0; i < Enemies.size(); i++) {
		for (size_t o = i; o < Obstacle.size(); o++) {

		}
	}

}
