#pragma once
#include "Player.h"

enum enemyType {
	TURRET, GHOST, PROJECTILE, SPIKES, SNARE
};

class Enemy : public GameObject {
public:
	Enemy(ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale);
	virtual void collidedWithPlayer() = 0;
	virtual void update(float dt) = 0;
	void lookat(vec3 lookat, vec3 offset = vec3(0,0,0));
private:

};