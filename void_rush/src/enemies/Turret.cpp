#include "Turret.h"

Turret::Turret(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot, vec3 scale):
	Enemy(file, gfx, pos, rot, scale)
{
	this->player = player;
	this->gfx = gfx;
	//setting values
	range = enemy_conf.Turret_range;
	shootCD = enemy_conf.Turret_CD;
	currentTimeTillShoot = 0.f;
}

void Turret::collidedWithPlayer()
{
	//nothing
}

void Turret::update(float dt)
{
#ifdef _DEBUG
	//Debug we can change these value without restarting...
	range = enemy_conf.Turret_range;
	shootCD = enemy_conf.Turret_CD;
#endif // DEBUG

	//if player is in range
	if ((getPos() - player->getPos()).length() < range) {
		//look at player
		lookat(player->getPos());
		//check if we should fire (cd)
		currentTimeTillShoot += dt;
		if (currentTimeTillShoot > shootCD) {
			currentTimeTillShoot = 0.f;
			bool done = false;
			for (size_t i = 0; i < projectiles.size() && !done; i++) {
				if (!projectiles[i]->isActive()) {
					sm->playSound("TurrShot",getPos());
					projectiles[i]->setActive(true);
					projectiles[i]->setDirection(player->getPos() - getPos());
					projectiles[i]->setPos(getPos());
					done = true;
				}
			}

		}
	}
}

void Turret::addProjectiles(TurrProjectile* projectile)
{
	projectiles.push_back(projectile);
}

void Turret::setSoundManager(SoundManager& sm)
{
	this->sm = &sm;
}
