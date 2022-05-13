#include "Snare.h"

Snare::Snare(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot, vec3 scale)
	:Enemy(file, gfx, pos, rot, scale)
{
	this->player = player;
	this->gfx = gfx;
	this->activated = false;
	this->reset = false;
	this->timer = 3.0f;
	this->resetTimer = 3.0f;
	this->catchTimer = 0.1f;
}

Snare::~Snare()
{
}

void Snare::update(float dt)
{
	tempDt = dt;
	if (reset) {
		Reset(dt);
		return;
	}
	if (!activated) {
		return;
	}
	this->player->setPos(this->PlayerPos);
	timer -= dt;
	if (timer < 0) {
		timer = 3.0f;
		resetTimer = 3.0f;
		catchTimer = 0.1f;
		activated = false;
		reset = true;
	}
}

void Snare::collidedWithPlayer()
{
	if (!activated && !reset && catchTimer <= 0.0f) {
		activated = true;
		this->PlayerPos = this->player->getPos();
	}
	if(catchTimer>0.0f && !reset)
		catchTimer -= tempDt;
}

void Snare::Reset(float dt)
{
	resetTimer -= dt;
	if (resetTimer <= 0) {
		reset = false;
	}
}
