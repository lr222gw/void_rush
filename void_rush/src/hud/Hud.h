#pragma once
#include "3Dproj/UIManager.h"

class Hud
{
private:
	UIManager* UI;
	ResourceManager* rm;
	Graphics* gfx;

	int currentHealth = 3;

	const int HEALTH = 0;
	const int GHOSTFRAME = 1;
	const int GHOSTBAR = 2;
	const int SCORE = 3;
	const int POWERUP = 4;
	const int PASSIVE1 = 5;
	const int PASSIVE2 = 6;
	const int PASSIVE3 = 7;
	const int PASSIVE4 = 8;

public:
	Hud(Graphics*& gfx, ResourceManager*& rm);
	virtual ~Hud();

	void setUpUI();
	void ResetHUD();
	void LowerHealth();
	void IncreaseHealth();

	void Update();
};

