#include "Projectile.h"

TurrProjectile::TurrProjectile(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot, vec3 scale) :
	Enemy(file, gfx, pos, rot, scale),
	direction(0.f, 0.f, 0.f),
	speed(20.f),
	active(false),
	TimeActive(4.f),
	currentTimeActive(0)
{
	this->player = player;
}

void TurrProjectile::collidedWithPlayer()
{
	if (active) {
		player->shovePlayer(vec2(1.f, 1.f), 5.f);//just test
	}
}

void TurrProjectile::update(float dt)
{
	if (currentTimeActive > TimeActive) {
		active = false;
		currentTimeActive = 0;
	}
	if (active) {
		this->movePos(direction * speed * dt);
		currentTimeActive += dt;
		
	}
}

bool TurrProjectile::isActive() const
{
	return active;
}

void TurrProjectile::setActive(bool active)
{
	this->active = active;
}

void TurrProjectile::setDirection(vec3 dir)
{
	this->direction = dir.Normalize();
}

void TurrProjectile::draw(Graphics*& gfx, bool sm)
{
	if (active) {
		GameObject::draw(gfx, sm);
	}
}
