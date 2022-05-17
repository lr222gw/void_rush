#pragma once
#include "enemies/Enemy.h"

class FallingPlatform : public Enemy {
public:
	FallingPlatform(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	~FallingPlatform();
	void update(float dt);
	void collidedWithPlayer();
private:
	void reset();
private:
	friend class ImguiManager;
	Player* player;
	Graphics* gfx;
	
	vec3 OrigPos;
	float fallTimerOrig;
	float fallTimer;
	float fallSpeed;
	bool trigger;
	bool holdPlayer;
};