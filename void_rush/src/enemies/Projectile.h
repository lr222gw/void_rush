#pragma once
#include "Enemy.h"

class TurrProjectile : public Enemy {
public:	
	TurrProjectile(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot, vec3 scale);
	void collidedWithPlayer();
	void update(float dt);
	bool isActive() const;
	void setActive(bool active);
	void setDirection(vec3 dir);
private:
	Player* player;
	vec3 direction;
	float speed;
	bool active;
	float TimeActive;
	float currentTimeActive;
	void draw(Graphics*& gfx, bool sm);
};