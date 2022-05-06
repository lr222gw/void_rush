#include "Turret.h"

Turret::Turret(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot, vec3 scale):
	Enemy(file, gfx, pos, rot, scale)
{
	this->player = player;
	this->gfx = gfx;
	//setting values
	range = 40;
	shootCD = 5;
	currentTimeTillShoot = 0;
}

void Turret::collidedWithPlayer()
{
	//nothing
}

void Turret::update(float dt)
{
	//if player is in range
	if ((getPos() - player->getPos()).length() < range) {
		//look at player

		//check if we should fire (cd)
		currentTimeTillShoot += dt;
		if (currentTimeTillShoot > shootCD) {
			currentTimeTillShoot = 0;
			//fire by throwing a projectile

		}
	}
}

void Turret::lookat()
{
}
