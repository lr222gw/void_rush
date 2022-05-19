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
	friend class ImguiManager;
	friend class EnemyManager;
	Player* player;
	Graphics* gfx;
	std::vector<TurrProjectile*> projectiles;
	//have a vector of projectiles
private:
	SoundManager* sm;
	float range;
	float shootCD;
	float currentTimeTillShoot;

	static struct Turret_settings {
		int Turret_maxNrOfProjectiles = 5;
		float Turret_y_Offset = 1.f;
		float Turret_CD = 6.f;
		float Turret_range = 40.f;
		float Turret_scale = 0.5f;

		float Projectile_scale = 0.4f;
	}turret_conf;
};