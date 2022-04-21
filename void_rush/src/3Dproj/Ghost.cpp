#include "Ghost.h"

Ghost::Ghost(Player* player, ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale):
	Enemy(file, gfx, pos, rot, scale)
{
	this->player = player;
	attackCD = 0.0f;
	readyToAttack = true;
	speed = 4;
	getPlayerPosCD = 0;
	rangeToPlayerBeforeNearestWay = 10;
	rangeToPointBeforeNewPoint = 3;
	this->active = false;
}

void Ghost::collidedWithPlayer()
{
	if (readyToAttack) {
		std::cout << "Player loses a life" << std::endl;
		readyToAttack = false;
		attackCD = 5.0f;
		player->TakeDmg();
	}
	
}

void Ghost::update(float dt)
{
	if (active) {
		if (!readyToAttack) {
			attackCD -= dt;
			if (attackCD < 0) {
				readyToAttack = true;
			}
		}
		followPlayer(dt);
	}
}

void Ghost::setActive(bool activate)
{
	this->active = activate;
}

void Ghost::followPlayer(float dt)
{
	if (!checkIfRangeOfPlayer()) {
		setRot(vec3(1.57f, 0, 0));
		getPlayerPosCD -= dt;
		if (getPlayerPosCD < 0) {
			getPlayerPosCD = 3;
			PlayerPositions.push(player->getPos());
			std::cout << "shot" << std::endl;
		}
		while (PlayerPositions.size() > 30) {
			PlayerPositions.pop();
		}
		
	} 
	if (!checkIfRangeOfPlayer() && !PlayerPositions.empty()) {
		vec3 ghostToPoint = (PlayerPositions.front() - getPos()).Normalize();
		this->movePos(ghostToPoint * dt * speed);
	}
	else{
		setRot(vec3(0, 0, 0));
		if (!PlayerPositions.empty()) {
			PlayerPositions.pop();
		}
		//go to player
		vec3 ghostToPlayer = (player->getPos() - getPos()).Normalize();
		this->movePos(ghostToPlayer * dt * speed);
	}
	if (checkIfInRangeOfPoint()) {
		PlayerPositions.pop();
	}
}

bool Ghost::checkIfInRangeOfPoint()
{
	if (PlayerPositions.empty()) {
		return false;
	}
	vec3 ghostToPoint = (PlayerPositions.front() - getPos());
	return ghostToPoint.length() < rangeToPointBeforeNewPoint;
}

bool Ghost::checkIfRangeOfPlayer()
{
	vec3 ghostToPlayer = (player->getPos() - getPos());
	return ghostToPlayer.length() < rangeToPlayerBeforeNearestWay;
}