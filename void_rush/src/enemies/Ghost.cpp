#include "Ghost.h"
#include "3Dproj/flags.h"

Ghost::Ghost(Player* player, ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale):
	Enemy(file, gfx, pos, rot, scale)
{
	rangeToPlayerBeforeNearestWay = 5;
	rangeToPointBeforeNewPoint = 0.5;
	this->Ghosts_Time = 0.0f;
	this->ghost_Time_interval = 20.f;
	this->ghost_max_dist_to_player = 40.f;
	this->speed_increase = 0.2f;
	this->force = vec3(10.0f, 3.0f, 10.f);
	this->initalSpeed = player->getSpeed() + 1.2f;
	this->speed = this->initalSpeed;
	this->player = player;
	this->frozen = false;
	this->active = false;
	this->attackCD = 0.0f;
	Reset();
	if (!(DEVMODE_ || DEBUGMODE))
	{ 
		active = true;
	}
	
}

void Ghost::collidedWithPlayer()
{
	if (readyToAttack) {
		std::cout << "Player loses a life" << std::endl;
		readyToAttack = false;
		attackCD = 1.0f;
		vec2 ghostToPlayer = (vec2(player->getPos().x, player->getPos().z) - vec2(getPos().x, getPos().z)).Normalize();
		vec2 shove = vec2(this->force.x * ghostToPlayer.x, this->force.z * ghostToPlayer.y);
		if (shove.legth() == 0) {
			shove = vec2(1, 1).Normalize();
		}
		player->shovePlayer(shove, this->force.y * 2);
	}
}

void Ghost::update(float dt)
{
	if (active && !this->frozen) {
		if (!readyToAttack) {
			attackCD -= dt;
			if (attackCD < 0) {
				readyToAttack = true;
			}
		}
		GainSpeed(dt);
		if (attackCD <= 0)
		{
			followPlayer(dt);
		}
		if (player->ResetGhost()) {
			this->Reset();
		}
		currentTimeToMakeNoice += dt;
		if (currentTimeToMakeNoice > TimeToMakeNoice) {
			sm->playSound(sounds[whatNoice]);
			whatNoice++;
			whatNoice %= 3;
			currentTimeToMakeNoice = 0;
		}
		for (int i = 0; i < 3; i++) {
			sm->updatePositionOfSound(getPos(), sounds[i]);
		}
		//Adjusting players bpm and game music based on distance
		float bpm = 60;
		float musicVol = 0.0f;
		float minDist = 1.0f;
		float maxDist = 70.0f;
		float len = fabs((this->getPos() - player->getPos()).length());
		if (len < maxDist) {
			player->EnableHeart();
			if (len < minDist) {
				len = minDist;
			}
			float part = minDist / len;
			bpm = 60 + part * 150.0f;
			musicVol = 5 + part*40.0f;
		}
		else {
			player->DisableHeart();
		}
		player->setBpm(bpm);
		player->setMusicVol(musicVol);
	}
	else {
		
		player->DisableHeart();
		player->setMusicVol(0.0f);
	}
}

void Ghost::setActive(bool activate)
{
	this->active = activate;
}

void Ghost::Reset()
{
	setPos(vec3(player->getPos().x, player->getPos().y, player->getPos().z - 30.f));
	readyToAttack = true;
	speed = this->initalSpeed;
	getPlayerPosCD = 1;
	if (!PlayerPositions.empty())
	{
		std::queue<vec3> empty;
		PlayerPositions.swap(empty);
	}
}

void Ghost::getSoundManager(SoundManager& sm)
{
	sounds[0] = "G0";
	sounds[1] = "G1";
	sounds[2] = "G2";

	sm.loadSound("assets/audio/Ghost1.wav", 10, sounds[0]);
	sm.loadSound("assets/audio/Ghost2.wav", 10, sounds[1]);
	sm.loadSound("assets/audio/Ghost3.wav", 10, sounds[2]);
	GameObject::getSoundManager(sm);
}

void Ghost::freezeGhost()
{
	if (this->frozen == false)
	{
		this->frozen = true;
	}
	else
	{
		this->frozen = false;
	}
}

bool Ghost::isFrozen()
{
	return this->frozen;
}

void Ghost::followPlayer(float dt)
{
	
	if (!checkIfRangeOfPlayer()) {
		
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
		lookat(PlayerPositions.front(), vec3(3.14f, -1.57f,0));
	}
	else{
		if (!PlayerPositions.empty()) {
			PlayerPositions.pop();
		}
		//go to player
		vec3 ghostToPlayer = (player->getPos() - getPos()).Normalize();
		this->movePos(ghostToPlayer * dt * speed);
		lookat(player->getPos(), vec3(3.14f, -1.57f, 0));
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

//Increseas the ghost speed during the game.
void Ghost::GainSpeed(float dt)
{
	float distToPlayer = (this->player->getPos() - this->getPos()).length();

	if (distToPlayer > this->ghost_max_dist_to_player) {
		this->speed = 10.f;
	}
	else {
		this->speed = this->initalSpeed;
	}

	
	
}