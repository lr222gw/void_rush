#pragma once
#include "Enemy.h"
#include <queue>

class Ghost : public Enemy {
public:
	Ghost(Player* player, ModelObj* file, Graphics*& gfx, vec3 pos = vec3(0,0,0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	void collidedWithPlayer();
	void update(float dt) override;
	void setActive(bool activate = true);
	void setInitialSpeed(float speed);
	void Reset();
	void getSoundManager(SoundManager& sm);
	//Functions for freeze power
	void freezeGhost();
	bool isFrozen();

private:
	friend class ImguiManager;
	void followPlayer(float dt);
	bool checkIfInRangeOfPoint();
	bool checkIfRangeOfPlayer();
	void GainSpeed(float dt);

	std::queue<vec3> PlayerPositions;
	float attackCD;
	float rangeToPlayerBeforeNearestWay;
	float rangeToPointBeforeNewPoint;
	float speed;
	float initalSpeed; 
	float getPlayerPosCD;
	float Ghosts_Time;
	float ghost_Time_interval; 
	float ghost_max_dist_to_player;
	float speed_increase;
	bool readyToAttack;
	bool fastestWayToPlayer;

	int whatNoice = 0;
	float TimeToMakeNoice = 9.0f;
	float currentTimeToMakeNoice = 0;

	//Variable for freezing ghost
	bool frozen;

	//Variables for shoving player
	vec3 force;
	std::string sounds[3];
	bool active;
	Player* player;
};