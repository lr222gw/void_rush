#include "FallingPlatform.h"

FallingPlatform::FallingPlatform(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot, vec3 scale)
	:Enemy(file, gfx, pos, rot, scale)
{
	this->player = player;
	this->gfx = gfx;


	this->OrigPos = pos;
	this->fallSpeed = 0.04f;
	this->fallTimerOrig = 3.0f;
	this->fallTimer = this->fallTimerOrig;
	this->trigger = false;
	this->holdPlayer = false;
}

FallingPlatform::~FallingPlatform()
{
}

void FallingPlatform::update(float dt)
{
	
	if(this->trigger){
		if (fallTimer <= 0.0f) {
			reset();
		}
		else {
			fallTimer -= dt;
			this->movePos(vec3(0.0f, -fallSpeed, 0.0f));
		}
	}
	else {
		this->OrigPos = this->getPos();
	}
	if (holdPlayer) {
		vec3 playerPos = this->player->getPos();
		this->player->movePos(vec3(0.0f, -fallSpeed, 0.0f));
		if (this->player->GetKB()->isKeyPressed(VK_SPACE)) {
			holdPlayer = false;
		}
	}
}

void FallingPlatform::collidedWithPlayer()
{
	if (!this->trigger) {
		this->trigger = true;
		this->holdPlayer = true;
	}
}

void FallingPlatform::reset()
{
	this->fallTimer = this->fallTimerOrig;
	this->trigger = false;
	this->holdPlayer = false;
	this->setPos(this->OrigPos);
}
