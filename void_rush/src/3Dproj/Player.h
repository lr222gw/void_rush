#pragma once
#include "GameObject.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Camera.h"
#include "common/Vector.hpp"
#include "hud/Hud.h"


#include "generation/Position_generator.hpp"//To use difficulty
#include "score/ScoreManager.hpp"

#include <string>
#include <fstream>

enum Powerup
{
	EMPTY,
	ROCKET,
	CARD,
	FREEZE,
	DEATH,
	EMP,
	PAD,
	APPLE
};


class Player : public GameObject {
public:
	Player(ModelObj* file, Graphics*& gfx, Camera*& cam, Mouse* mouse, Keyboard* keyboard, Hud* HUD, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	virtual ~Player();
	void update(float dt) override;
	void handleEvents(float dt);
	void rotateWithMouse(int x, int y);
	void addRot(vec3 rot);
	void setGrounded();
	void setUngrounded();
	float getSpeed();
	float getGroundedTimer();
	GameObject*& getPlayerObjPointer();
	void Reset(bool lvlClr = false);

	//Used when player falls of platform to rest ghost
	bool ResetGhost();
	void shovePlayer(vec2 shove, float forceY);

	//Powerup function
	void pickedUpPower(Powerup index);
	Powerup getPlayerPower();
	void usePowerup();

	void SetDifficulity(Difficulity diff);
	void SetStartPlatform(Platform*& start);
	void writeScore(std::string file = "assets/files/highScores.txt");

	void AddToName(unsigned char letter);
	void RemoveLetter();
	std::string GetName()const;
	int GetMaxLetters();
	int GetCurrentLetter();
	void ResetName();
	bool GetSubmitName();
	void SetSubmitName(bool val);

	void SetCurrentSeed(int seed);

	
private:
	friend class ImguiManager;
	bool noClip;
	bool invincible;
	void Translate(float dt, DirectX::XMFLOAT3 translate);
	vec3 speed;
	float jumpForce;
	float midAirAdj;
	vec3 velocity;
	vec3 acceleration;
	vec3 resForce;
	vec3 gravity;
	vec2 jumpDir;
	float mass;
	bool grounded;
	float groundedTimer;

	//For being shoved
	bool shoved;
	vec2 shove;

	//Powerups
	Powerup power_index;

	vec2 startingJumpDir = vec2(0.0f, 0.0f);
	char startingJumpKey = 'N';
	bool isKeyPressed = false;
	bool resetGhost;

	Mouse* mouse;
	Keyboard* keyboard;
	Camera* cam;

	ScoreManager scoreManager;
	std::string name;
	int maxLetters;
	int currentLetter;
	bool submitName;
	//float levelTime;
	vec3 puzzlePos;
	Difficulity levelDifficulty;
	Hud* HUD;
	

	float score;
	float health;
	bool alive;
	float maxDepth;

	
public:
	void TakeDmg(int dmg = 1);
	void AddHealth(int hlt = 1);
	void AddScore(float scr = 1.0f);
	int GetHealth();
	float GetScore();
	bool IsAlive();
	GameObject* GOPTR; //GameObjectPlayerPointer
};