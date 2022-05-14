#pragma once
#include "enemies/Enemy.h"
#include <iostream>
#include "3Dproj/Vec.h"

class Snare : public Enemy {
public:
	Snare(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	~Snare();
	void update(float dt);
	void collidedWithPlayer();
private:
	void Reset(float dt);
	Player* player;
	Graphics* gfx;
	float timer;
	float resetTimer;
	float catchTimer;
	bool reset;
	bool activated;
	vec3 PlayerPos;
	float tempDt;
};