#pragma once
#include "enemies/Projectile.h"
#include "EnemySettings.hpp"
#include <vector>

class Turret : public Enemy {
public:
	Turret(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot = vec3(0,0,0), vec3 scale = vec3(1, 1, 1));
	void collidedWithPlayer();
	void update(float dt);
	void addProjectiles(TurrProjectile* projectile);
	void setSoundManager(SoundManager& sm);
private:
	Player* player;
	Graphics* gfx;
	std::vector<TurrProjectile*> projectiles;
	//have a vector of projectiles
private:
	SoundManager* sm;
	float range;
	float shootCD;
	float currentTimeTillShoot;
};