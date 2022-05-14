#pragma once
#include "3Dproj/Player.h"


enum class enemyType {
	GHOST, TURRET, PROJECTILE, SPIKES, SNARE, _COUNT
}; 

//NOTE: _COUNT is used to determine how many types of enemies there are...
enum class respawnable_enemyType {
	TURRET, SPIKES, SNARE, _COUNT
}; 

class Enemy : public GameObject {
public:
	Enemy(ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale);
	virtual void collidedWithPlayer() = 0;
	virtual void update(float dt) = 0;
	void lookat(vec3 lookat, vec3 offset = vec3(0,0,0));
private:

};