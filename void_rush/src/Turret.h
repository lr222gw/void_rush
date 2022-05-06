#pragma once
#include "3Dproj/Projectile.h"
#include <vector>

class Turret : public Enemy {
public:
	Turret(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot = vec3(0,0,0), vec3 scale = vec3(1, 1, 1));
	void collidedWithPlayer();
	void update(float dt);
	void addProjectiles(TurrProjectile* projectile);
private:
	Player* player;
	Graphics* gfx;
	std::vector<TurrProjectile*> projectiles;
	//have a vector of projectiles
private:
	float range;
	float shootCD;
	float currentTimeTillShoot;
	void lookat();
};