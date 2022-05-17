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
		vec3 shf = (player->getPos() - getPos()).Normalize() * 5;
		player->shovePlayer(vec2(shf.x, shf.z), 10.f);
		active = false;
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
	lookat(getPos() + direction);
}

void TurrProjectile::draw(Graphics*& gfx, bool sm)
{
	if (active) {
		GameObject::draw(gfx, sm);
	}
}
