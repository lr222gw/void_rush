#include "CollisionHandler.h"

CollisionHandler::CollisionHandler()
	:lastCollided_ShapePlatform(nullptr)
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

void CollisionHandler::addPearl(GameObject* pearl)
{
	this->Pearl = pearl;
}

void CollisionHandler::addPowerups(Powerups* powerups)
{
	this->powerups.push_back(powerups);
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

void CollisionHandler::deletePowerups(Powerups* ptr)
{
	for (size_t i = 0; i < powerups.size(); i++) {
		if (powerups[i] == ptr) {
			powerups.erase(std::next(powerups.begin(), i));
		}
	}
}

void CollisionHandler::update()
{
	//check player with Custom platforms (Pussels and other(?) handmade)
	bool done = false;
	
	//Fall box
	DirectX::XMFLOAT4 bb[2];
	ColCube obj;

	for (size_t i = 0; i < Custom_platforms.size(); i++) {
		if (!done && collision3D(player->getPlayerObjPointer(), Custom_platforms[i], true, false))
		{
			done = true;

			if (player->getGroundedTimer() > 0.01f)
			{
				player->setGrounded();
			}
		}
		collisionWithBlocking(Custom_platforms[i], player);


		//Player scream / fallbox check
		Custom_platforms[i]->getBoundingBox(bb);
		obj.highPoint = vec3(bb[1].x, bb[1].y, bb[1].z);
		obj.lowPoint = vec3(bb[0].x, bb[0].y, bb[0].z);
		if (collision3D(player->getFallCube(), obj)) {
			player->ResetFallBoxTimer();
		}
	}

	//Check player with generated platforms	
	DirectX::XMFLOAT4 player_bounding_box[2];	
	player->getPlayerObjPointer()->getBoundingBoxFromObject(player_bounding_box);
	for (size_t i = 0; i < Generated_Platforms.size()&& !done ; i++) {
		
		for (size_t j = 0; j < Generated_Platforms[i]->bounding_boxes.size()&& !done; j++ ) {

			DirectX::XMFLOAT4 min_max_bounds[2]{};
			
			min_max_bounds[0] = {Generated_Platforms[i]->bounding_boxes[j].first.x,
				Generated_Platforms[i]->bounding_boxes[j].first.y,
				Generated_Platforms[i]->bounding_boxes[j].first.z,
				1.f};
			min_max_bounds[1] = { Generated_Platforms[i]->bounding_boxes[j].second.x,
				Generated_Platforms[i]->bounding_boxes[j].second.y,
				Generated_Platforms[i]->bounding_boxes[j].second.z,
				1.f };

			
			if (!done && collision3D(player_bounding_box, min_max_bounds))
			{
				done = true;
				this->lastCollided_ShapePlatform = Generated_Platforms[i];
				if (player->getGroundedTimer() > 0.01f )
				{
					player->setGrounded();
				}
			}

			collisionWithBlocking(min_max_bounds, player);

			//Player scream / fallbox check
			obj.highPoint = Generated_Platforms[i]->bounding_boxes[j].first;
			obj.lowPoint = Generated_Platforms[i]->bounding_boxes[j].second;
			if (collision3D(player->getFallCube(), obj)) {
				player->ResetFallBoxTimer();
			}

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
		else if (collision3D(player->getPlayerObjPointer(), Enemies[i], true, false)) {
			Enemies[i]->collidedWithPlayer();
		}
		Enemies[i]->getBoundingBox(bb);
		obj.highPoint = vec3(bb[1].x, bb[1].y, bb[1].z);
		obj.lowPoint = vec3(bb[0].x, bb[0].y, bb[0].z);
		if (collision3D(player->getFallCube(), obj)) {
			player->ResetFallBoxTimer();
		}
		
	}
	
	//check player with Obstacle // different things happens based on what Obstacle it is
	for (size_t i = 0; i < Obstacle.size(); i++) {

	}

	//check enemies with platform
	for (size_t i = 0; i < Enemies.size(); i++) {
		for (size_t o = i; o < Custom_platforms.size(); o++) {
			//check if projectile hit platform if it does disapear
		}
	}

	//check enemies with Obstacle // don't know what happens here
	//for (size_t i = 0; i < Enemies.size(); i++) {
	//	for (size_t o = i; o < Obstacle.size(); o++) {
	//
	//	}
	//}

	for (size_t i = 0; i < powerups.size(); i++)
	{
		if (collision3D(player->getPlayerObjPointer(), powerups[i]))
		{
			player->pickedUpPower(powerups[i]->getPowerUpIndex());
			//TODO: powerUP begone (no draw)
			powerups[i]->setPos(vec3(1000.f, 1000.f, 1000.f));
			powerups[i]->setWannaDraw(false);
			powerups[i]->setPickedUp(true);
		}
	}

	if (player->getPearlStatus())
	{
		done = false;
		DirectX::XMFLOAT4 pearl_bounding_box[2];
		player->GetPearl()->getBoundingBoxFromObject(pearl_bounding_box);
		for (size_t i = 0; i < Generated_Platforms.size() && !done; i++) {

			for (size_t j = 0; j < Generated_Platforms[i]->bounding_boxes.size() && !done; j++) {

				DirectX::XMFLOAT4 min_max_bounds[2]{};

				min_max_bounds[0] = { Generated_Platforms[i]->bounding_boxes[j].first.x,
					Generated_Platforms[i]->bounding_boxes[j].first.y,
					Generated_Platforms[i]->bounding_boxes[j].first.z,
					1.f };
				min_max_bounds[1] = { Generated_Platforms[i]->bounding_boxes[j].second.x,
					Generated_Platforms[i]->bounding_boxes[j].second.y,
					Generated_Platforms[i]->bounding_boxes[j].second.z,
					1.f };

				if (!done && collision3D(pearl_bounding_box, min_max_bounds))
				{
					done = true;
					player->PearlHit();
				}
			}
		}
		for (size_t i = 0; i < Custom_platforms.size(); i++) {
			if (!done && collision3D(player->GetPearl(), Custom_platforms[i], true, false))
			{
				done = true;
				player->PearlHit();
			}
		}
	}
}
