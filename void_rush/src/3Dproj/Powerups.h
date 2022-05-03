#pragma once
#include "GameObject.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Camera.h"
#include "common/Vector.hpp"
#include "hud/Hud.h"
#include "Player.h"
//ROCKET = 1;
//CARD = 2;
//FREEZE = 3;
//DEATH = 4;
//EMP = 5;
//PAD = 6;

enum powerups
{
	EMPTY,
	ROCKET,
	CARD, 
	FREEZE,
	DEATH,
	EMP, 
	PAD
};


class Powerups:public GameObject
{
public:
	Powerups(Player * player, Hud * hud, ModelObj* file, Graphics*& gfx, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	virtual ~Powerups();

	void UsePowerUp();
	
private:


	Hud* hud;
	Player* player;
};
