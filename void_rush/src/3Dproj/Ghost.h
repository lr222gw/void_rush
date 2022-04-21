#pragma once
#include "Enemy.h"
#include <queue>

class Ghost : public Enemy {
public:
	Ghost(Player* player, ModelObj* file, Graphics*& gfx, vec3 pos = vec3(0,0,0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	void collidedWithPlayer();
	void update(float dt) override;
	void setActive(bool activate = true);
private:
	void followPlayer(float dt);
	bool checkIfInRangeOfPoint();
	bool checkIfRangeOfPlayer();

	std::queue<vec3> PlayerPositions;
	float attackCD;
	float rangeToPlayerBeforeNearestWay;
	float rangeToPointBeforeNewPoint;
	float speed;
	float getPlayerPosCD;
	bool readyToAttack;
	bool fastestWayToPlayer;

	bool active;
	Player* player;
};