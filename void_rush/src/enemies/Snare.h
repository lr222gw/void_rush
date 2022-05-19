#pragma once
#include "enemies/Enemy.h"
#include <iostream>
#include "3Dproj/Vec.h"
#include "enemies/EnemySettings.hpp"

class Snare : public Enemy {
public:
	Snare(ModelObj* file, Graphics*& gfx, Player* player, vec3 pos, vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	~Snare();
	void update(float dt);
	void collidedWithPlayer();
private:
	friend class ImguiManager;
	friend class EnemyManager;
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

	static struct Snare_settings { 

		float Snare_y_Offset = .25f;
		float Snare_timer = 3.f;
		float Snare_catchTimer = 0.1f;
		float Snare_scale = 0.3f;
		float Snare_scale_y = 0.2f;
	}snare_conf;
};