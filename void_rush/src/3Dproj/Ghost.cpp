#include "Ghost.h"

Ghost::Ghost(Player* player, ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale):
	Enemy(file, gfx, pos, rot, scale)
{
	rangeToPlayerBeforeNearestWay = 5;
	rangeToPointBeforeNewPoint = 0.5;
	this->Ghosts_Time = 0.0f;
	this->ghost_Time_interval = 20.f;
	this->speed_increase = 0.1f;
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
		//player->TakeDmg();
		vec3 ghostToPlayer = (player->getPos() - getPos()).Normalize();
		player->shovePlayer(vec3(50.f, 3.f, 50.f), vec2(ghostToPlayer.x, ghostToPlayer.z));
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
		GainSpeed(dt);
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
	setPos(vec3(player->getPos().x, player->getPos().y, player->getPos().z - 10.f));
	readyToAttack = true;
	speed = 1;
	getPlayerPosCD = 0;
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
		if (getPlayerPosCD < 0) {
			getPlayerPosCD = 3;
			if (PlayerPositions.empty())
			{
				PlayerPositions.push(player->getPos());
			}
			else
			{
				if ((player->getPos() - PlayerPositions.back()).length() > this->rangeToPointBeforeNewPoint)
				{
					PlayerPositions.push(player->getPos());
				}
			}
		}
		while (PlayerPositions.size() > 120) {
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

//check if ghost are within range of player
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

void Ghost::GainSpeed(float dt)
{
	if (this->speed < player->getSpeed())
	{
		if (this->Ghosts_Time >= this->ghost_Time_interval)
		{
			this->speed += this->speed_increase;
			this->Ghosts_Time = 0.0f;
		}
		else
		{
			this->Ghosts_Time += dt;
		}
	}
}


