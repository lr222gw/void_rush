#pragma once
#include "GameObject.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Camera.h"
#include "common/Vector.hpp"
#include "hud/Hud.h"
#include "Player.h"
#include "Ghost.h"


class Powerups:public GameObject
{
public:
	Powerups(ModelObj* file, Graphics*& gfx, Player* player, Ghost * ghost, Keyboard* keyboard, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), Powerup pow = EMPTY);
	virtual ~Powerups();
	void update(float dt) override;
	void UsePowerUp(float dt);
	Powerup getPowerUpIndex();

private:

	Keyboard* keyboard;
	Powerup power_index;
	Player* player;
	Ghost* ghost;
	float ghostFrozenTimer;
	bool featherActive;
	bool shieldActive;
};
