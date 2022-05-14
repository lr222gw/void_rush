#pragma once
#include "enemies/Enemy.h"
#include <iostream>

class Spikes : public Enemy {
public:
	Spikes(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), bool moving = false);
	~Spikes();
	void update(float dt);
	void collidedWithPlayer();
	void SetMoving(bool moving);
private:
	void move(float dt);
private:
	Player* player;
	Graphics* gfx;
	bool movingSpikes;
	bool moving;
	bool up;
	float timer;
	vec3 normalScale;
};