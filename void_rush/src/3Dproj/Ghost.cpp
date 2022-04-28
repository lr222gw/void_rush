#include "Ghost.h"

Ghost::Ghost(Player* player, ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale):
	Enemy(file, gfx, pos, rot, scale)
{
	/*this->player = player;
	attackCD = 0.0f;
	readyToAttack = true;
	speed = 4;
	getPlayerPosCD = 0;
	rangeToPlayerBeforeNearestWay = 10;
	rangeToPointBeforeNewPoint = 3;*/
	this->player = player;
	Reset();
	this->active = false;
}

void Ghost::collidedWithPlayer()
{
	if (readyToAttack) {
		std::cout << "Player loses a life" << std::endl;
		readyToAttack = false;
		attackCD = 5.0f;
		player->TakeDmg();
		this->Reset();
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
		if (player->ResetGhost()) {
			this->Reset();
		}
	}
}

void Ghost::setActive(bool activate)
{
	this->active = activate;
}

void Ghost::Reset()
{
	setPos(vec3(0.0f, 0.0f, -10.f));
	readyToAttack = true;
	speed = 1;
	getPlayerPosCD = 0;
	rangeToPlayerBeforeNearestWay = 10;
	rangeToPointBeforeNewPoint = 3;
	if (!PlayerPositions.empty())
	{
		std::queue<vec3> empty;
		PlayerPositions.swap(empty);
	}
	this->active = false;
}

void Ghost::followPlayer(float dt)
{
	if (!checkIfRangeOfPlayer()) {
		setRot(vec3(1.57f, 0, 0));
		getPlayerPosCD -= dt;
		if (getPlayerPosCD <= 0) {
			getPlayerPosCD = 2;
			PlayerPositions.push(player->getPos());
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

//check if distans to player is less then closest new point
bool Ghost::checkIfRangeOfPlayer()
{
	vec3 ghostToPlayer = (player->getPos() - getPos());
	return ghostToPlayer.length() < rangeToPlayerBeforeNearestWay;
}
