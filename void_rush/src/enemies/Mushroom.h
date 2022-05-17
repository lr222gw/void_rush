#pragma once
#include "enemies/Enemy.h"
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
	friend class ImguiManager;
	float forceY;
	float force;
	float scaleTimer;
	vec3 origScale;
	Player* player;
	Graphics* gfx;
	vec3 pushVec;
	

	/*static struct Mushroom_settings {
		float forceY = 20.0f;
		float force = 10.0f;
	}mushroom_conf;*/
};