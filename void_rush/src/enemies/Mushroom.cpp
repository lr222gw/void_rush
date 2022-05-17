#include "Mushroom.h"

//Mushroom::Mushroom_settings Mushroom::mushroom_conf;

Mushroom::Mushroom(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot, vec3 scale)
	:Enemy(file, gfx, pos, rot, scale)
{
	this->player = player;
	this->gfx = gfx;
	this->pushVec = vec3(0.0f, 0.0f, 0.0f);
	this->force = 10.f;//Mushroom::mushroom_conf.force;
	this->forceY = 20.f;//Mushroom::mushroom_conf.forceY;
	this->scaleTimer = 0.0f;
	this->origScale = this->getScale();
}

Mushroom::~Mushroom()
{
}

void Mushroom::update(float dt)
{
	resetScoreTime += dt;
	if (scaleTimer > 0.0f) {
		scaleTimer -= dt;
	}
	else {
		this->setScale(this->origScale + vec3(-0.1f, 0.0f, -0.1f));
	}

}

void Mushroom::collidedWithPlayer()
{
	vec3 horPush = GetMidPos(player) - GetMidPos(this);
	float objPlayerLen = (vec2(GetMidPos(player).x, GetMidPos(player).z) - vec2(GetMidPos(this).x, GetMidPos(this).z)).legth();
	horPush.y = 0.0f;

	DirectX::XMFLOAT4 bb[2];
	this->getBoundingBox(bb);
	float length = fabs(bb[0].x - bb[1].x);
	float radius = length / 2.0f;

	if (objPlayerLen > radius) {
		return;
	}

	float howMuch = objPlayerLen / radius;
	if (howMuch > 1.0f) {
		howMuch = 1.0f;
	}

	

	float yPush = (1 - howMuch) * forceY;
	horPush = horPush * force;

	

	//player->shovePlayer(vec2(horPush.x, horPush.z), yPush);
	player->bouncePlayer(vec2(horPush.x, horPush.z), yPush);
	player->ResetFallBoxTimer();

	
	if (resetScoreTime > 1.f) {

		player->AddScore(100);
		resetScoreTime = 0.f;
	}

	this->setScale(this->origScale + vec3(0.1f, 0.0f, 0.1f));
	this->scaleTimer = 0.2f;
	
}
