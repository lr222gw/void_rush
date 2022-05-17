#pragma once
#include "enemies/Enemy.h"
#include "EnemySettings.hpp"
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
	friend class ImguiManager;
	friend class EnemyManager;
	Player* player;
	Graphics* gfx;
	bool movingSpikes;
	bool moving;
	bool up;
	float timer;
	vec3 normalScale;

	static struct Spikes_settings {

		float Spikes_y_Offset = 0.2f;
		float Spikes_timer = 2.f;
		float Spikes_scale = 0.0001f;

	}spikes_conf;
};