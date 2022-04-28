#pragma once
#include "Enemy.h"
#include <queue>

class Ghost : public Enemy {
public:
	Ghost(Player* player, ModelObj* file, Graphics*& gfx, vec3 pos = vec3(0,0,0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	void collidedWithPlayer();
	void update(float dt) override;
	void setActive(bool activate = true);
	void Reset();
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
	float getPlayerPosCD;
	float Ghosts_Time;
	float ghost_Time_interval; 
	float speed_increase;
	bool readyToAttack;
	bool fastestWayToPlayer;

	bool active;
	Player* player;
};