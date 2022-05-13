#include "Mushroom.h"

Mushroom::Mushroom(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot, vec3 scale)
	:Enemy(file, gfx, pos, rot, scale)
{
	this->player = player;
	this->gfx = gfx;
	this->pushVec = vec3(0.0f, 0.0f, 0.0f);
}

Mushroom::~Mushroom()
{
}

void Mushroom::update(float dt)
{
}

void Mushroom::collidedWithPlayer()
{
	vec3 horPush = GetMidPos(player) - GetMidPos(this);
	float objPlayerLen = (GetMidPos(player) - GetMidPos(this)).length();
	horPush.y = 0.0f;

	DirectX::XMFLOAT4 bb[2];
	this->getBoundingBox(bb);
	float length = (vec3(bb[0]) - vec3(bb[1])).length();
	float radius = length / 2;

	float howMuch = objPlayerLen / radius;
	if (howMuch > 1) {
		howMuch = 1;
	}

	float force = 10.0f;

	float yPush = (1 - howMuch) * objPlayerLen * force;
	horPush = horPush * force;

	player->shovePlayer(vec2(horPush.x, horPush.z), yPush);
	
}
