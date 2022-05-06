#include "Turret.h"

Turret::Turret(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot, vec3 scale):
	Enemy(file, gfx, pos, rot, scale)
{
	this->player = player;
	this->gfx = gfx;
	//setting values
	range = 40;
	shootCD = 4;
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
		lookat();
		//check if we should fire (cd)
		currentTimeTillShoot += dt;
		if (currentTimeTillShoot > shootCD) {
			currentTimeTillShoot = 0;
			bool done = false;
			for (size_t i = 0; i < projectiles.size() && !done; i++) {
				if (!projectiles[i]->isActive()) {
					projectiles[i]->setActive(true);
					projectiles[i]->setDirection(player->getPos() - getPos());
					projectiles[i]->setPos(getPos());
					done = true;
					std::cout << "shoot" << std::endl;
				}
			}

		}
	}
}

void Turret::addProjectiles(TurrProjectile* projectile)
{
	projectiles.push_back(projectile);
}

void Turret::lookat()
{
	vec3 forward(0, 0, 1);
	vec3 up(0, 1, 0);
	vec3 TurretToPlayer = player->getPos() - getPos();
	float xrot = vec3(TurretToPlayer.x, 0, TurretToPlayer.z).angle(forward);

	TurretToPlayer.Normalize();
	float yrot = acos(TurretToPlayer.Normalize() * up);

	this->setRot(vec3(yrot, -xrot, 0));

}
