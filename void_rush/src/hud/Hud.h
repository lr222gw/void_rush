#pragma once
#include "3Dproj/UIManager.h"

class Hud
{
private:
	UIManager* UI;
	ResourceManager* rm;
	Graphics* gfx;

	int currentHealth = 3;
	float fullBar = 0.467f;
	float noBar = 0.01f;

	const int HEALTH = 0;
	const int GHOSTBAR = 1;
	const int GHOSTFRAME = 2;
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
	void UpdateGhostBar(vec3 playerPos, vec3 puzzlePos, float totalDistance);

	void Update();
};

