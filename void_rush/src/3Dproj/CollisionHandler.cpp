#include "CollisionHandler.h"

CollisionHandler::CollisionHandler()
{
	
}

void CollisionHandler::addPlatform(GameObject* platform)
{
	this->Custom_platforms.push_back(platform);
}

void CollisionHandler::addPlatform(Shape* platform)
{
	this->Generated_Platforms.push_back(platform);
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
	for (size_t i = 0; i < Custom_platforms.size(); i++) {
		if (Custom_platforms[i] == ptr) {
			Custom_platforms.erase(Custom_platforms.begin() + i);
		}
	}
}

void CollisionHandler::deletePlatform(Shape* ptr)
{
	for (size_t i = 0; i < Generated_Platforms.size(); i++) {
		if (Generated_Platforms[i] == ptr) {
			Generated_Platforms.erase(Generated_Platforms.begin() + i);
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
	//check player with Custom platforms (Pussels and other(?) handmade)
	bool done = false;
	
	for (size_t i = 0; i < Custom_platforms.size(); i++) {
		if (!done && collision3D(player->getPlayerObjPointer(), Custom_platforms[i], true, false))
		{
			done = true;

			if (player->getGroundedTimer() > 0.5f)
			{
				player->setGrounded();
			}
		}
		collisionWithBlocking(Custom_platforms[i], player);
	}

	//Check player with generated platforms	
	DirectX::XMFLOAT4 player_bounding_box[2];	
	player->getPlayerObjPointer()->getBoundingBoxFromObject(player_bounding_box);
	for (size_t i = 0; i < Generated_Platforms.size()&& !done ; i++) {
		
		for (size_t j = 0; j < Generated_Platforms[i]->bounding_boxes.size()&& !done; j++ ) {

			DirectX::XMVECTOR min_max_vec[2] = {
				{Generated_Platforms[i]->bounding_boxes[j].first.x,
				Generated_Platforms[i]->bounding_boxes[j].first.y,
				Generated_Platforms[i]->bounding_boxes[j].first.z,
				1.f},
				{Generated_Platforms[i]->bounding_boxes[j].second.x,
				Generated_Platforms[i]->bounding_boxes[j].second.y,
				Generated_Platforms[i]->bounding_boxes[j].second.z,
				1.f}
			};

			DirectX::XMFLOAT4 min_max_bounds[2]{};
			
			DirectX::XMStoreFloat4(&min_max_bounds[0], min_max_vec[0]);
			DirectX::XMStoreFloat4(&min_max_bounds[1], min_max_vec[1]);

			
			if (!done && collision3D(player_bounding_box, min_max_bounds))
			{
				done = true;

				if (player->getGroundedTimer() > 0.5f )
				{
					player->setGrounded();
				}
			}

			collisionWithBlocking(min_max_bounds, player);
		}
	}
	if (!done)
	{
		player->setUngrounded();
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
		for (size_t o = i; o < Custom_platforms.size(); o++) {

		}
	}

	//check enemies with Obstacle // don't know what happens here
	for (size_t i = 0; i < Enemies.size(); i++) {
		for (size_t o = i; o < Obstacle.size(); o++) {

		}
	}

}
