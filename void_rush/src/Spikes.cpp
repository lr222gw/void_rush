#include "Spikes.h"

Spikes::Spikes(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot, vec3 scale, bool moving)
	:Enemy(file, gfx, pos, rot, scale)
{
	this->player = player;
	this->gfx = gfx;
	this->movingSpikes = moving;
	this->moving = false;
	this->up = true;
	this->timer = 2.0f;
	this->normalScale = this->getScale();
}

Spikes::~Spikes()
{
}

void Spikes::update(float dt)
{
	if (!movingSpikes) {
		return;
	}
	if (moving) {
		move(dt);
		return;
	}
	if (fabs((this->getPos() - player->getPos()).length()) > 10) {
		return;
	}
	timer -= dt;
	if (timer <= 0.0f) {
		moving = true;
		this->timer = 2.0f;
	}
}

void Spikes::collidedWithPlayer()
{
	if (!player->isInvinc()) {
		player->TakeDmg();
	}
}

void Spikes::SetMoving(bool moving)
{
	this->movingSpikes = moving;
}

void Spikes::move(float dt)
{
	if (up) {
		if (this->getScale().y > 0) {
			this->addScale(vec3(0.0f, this->normalScale.y * (-dt), 0.0f));
		}
		else {
			this->setScale(vec3(this->normalScale.x, 0.0f, this->normalScale.z));
			this->moving = false;
			this->up = false;
		}

	}
	else {
		if (this->getScale().y < this->normalScale.y) {
			this->addScale(vec3(0.0f, this->normalScale.y*dt, 0.0f));
		}
		else {
			this->setScale(this->normalScale);
			this->moving = false;
			this->up = true;
		}
	}
}

