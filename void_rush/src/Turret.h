#pragma once
#include "3Dproj/Enemy.h"

class Turret : public Enemy {
public:
	Turret(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot = vec3(0,0,0), vec3 scale = vec3(1, 1, 1));
	void collidedWithPlayer();
	void update(float dt);
private:
	Player* player;
	Graphics* gfx;
	//have a vector of projectiles
private:
	float range;
	float shootCD;
	float currentTimeTillShoot;
	void lookat();
};