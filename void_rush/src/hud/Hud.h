#pragma once
#include "3Dproj/UIManager.h"

class Hud
{
private:
	UIManager* UI;
	ResourceManager* rm;
	Graphics* gfx;

	int currentHealth = 3;
	float sizeOfBar = 0.0f;
	float fullBar = 0.467f;
	float noBar = 0.01f;

	const int HEALTH = 0;
	const int GHOSTBAR = 1;
	const int GHOSTFRAME = 2;
	const int GHOSTICON = 3;
	const int SCORE = 4;
	const int POWERUP = 5;
	const int PASSIVE1 = 6;
	const int PASSIVE2 = 7;
	const int PASSIVE3 = 8;
	const int PASSIVE4 = 9;

	bool passive1On = false;
	bool passive2On = false;
	bool passive3On = false;
	bool passive4On = false;

	int currentPowerUp = 0;
	const int EMPTY = 0;
	const int ROCKET = 1;
	const int CARD = 2;
	const int FREEZE = 3;
	const int DEATH = 4;
	const int EMP = 5;
	const int PAD = 6;

	//Rocket
	//Credit Card
	//Freeze
	//Skull
	//EMP
	//Trampoline


public:
	Hud(Graphics*& gfx, ResourceManager*& rm);
	virtual ~Hud();

	void setUpUI();
	void ResetHUD();
	void LowerHealth();
	void IncreaseHealth();
	void TurnOnPassive(int index);
	void TurnOffPassive(int index);
	void ChangeCurrentPowerUp(int index);
	bool GetStatusOfPassive(int index);
	void UpdateGhostBar(vec3 playerPos, vec3 puzzlePos, vec3 ghostPos, float totalDistance);

	void Update();
};

