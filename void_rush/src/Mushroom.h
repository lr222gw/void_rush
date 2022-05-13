#pragma once
#include "3Dproj/Enemy.h"
#include <iostream>
#include "3Dproj/Vec.h"
#include "interact/interact.hpp"

class Mushroom : public Enemy {
public:
	Mushroom(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	~Mushroom();
	void update(float dt);
	void collidedWithPlayer();
private:
	Player* player;
	Graphics* gfx;
	vec3 pushVec;
};