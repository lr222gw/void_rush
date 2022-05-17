#pragma once
#include "3Dproj/GameObject.h"
#include "3Dproj/Mouse.h"
#include "3Dproj/Keyboard.h"
#include "3Dproj/Camera.h"
#include "common/Vector.hpp"
#include "hud/Hud.h"
#include "3Dproj/Player.h"
#include "enemies/Ghost.h"
#include "powerups/PowerUpDefs.hpp"


class Powerups:public GameObject
{
public:
	Powerups(ModelObj* file, Graphics*& gfx, Player* player, Ghost * ghost, Mouse* mouse, Keyboard* keyboard, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), Powerup pow = EMPTY);
	virtual ~Powerups();
	void update(float dt) override;
	void UsePowerUp(float dt);
	Powerup getPowerUpIndex();

private:

	Keyboard* keyboard;
	Mouse* mouse;
	Powerup power_index;
	Player* player;
	Ghost* ghost;
	float ghostFrozenTimer;
	bool featherActive;
	bool pearlActive = false;
	float pearlTime = 0.0f;
	vec3 pearlVec;
	bool potionActive;
	float potionTimer;
	bool rocketActive;
	float rocketTimer;
	bool featherbooltest = false;

};
