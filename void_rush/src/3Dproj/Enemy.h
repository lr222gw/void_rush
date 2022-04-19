#pragma once
#include "Player.h"

class Enemy : public GameObject {
public:
	Enemy(ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale);
	virtual void collidedWithPlayer() = 0;
	virtual void update(float dt) = 0;
private:

};