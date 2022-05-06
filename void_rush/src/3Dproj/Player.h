#pragma once
#include "GameObject.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Camera.h"
#include "common/Vector.hpp"
#include "hud/Hud.h"


#include "generation/Position_generator.hpp"//To use difficulty
#include "score/ScoreManager.hpp"
#include "Collision3D.h"

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
	APPLE,
	FEATHER,
	PEARL,
	POTION,
	SHIELD,
	KILL,
	MONEY

};

//used with HUD
enum PowerUpPassiv
{
	FEATHER_P = 1,
	PEARL_P = 2,
	POTION_P = 3,
	SHIELD_P = 4
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
	ColCube getFallCube()const;
	void ResetFallBoxTimer();
	void Reset(bool lvlClr = false);

	//Used when player falls of platform to rest ghost
	bool ResetGhost();
	void shovePlayer(vec2 shove, float forceY);

	//Powerup function
	void pickedUpPower(Powerup index);
	Powerup getPlayerPower();
	void setPlayerPower(Powerup index);

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
	void getSoundManager(SoundManager& sm);
	SoundManager* getSm()const;

	void setBpm(float bpm);
	
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
	bool shoveDelay;
	float shoveTimer;
	//Beat
	float heartBeatTimer;
	float bpm;

	//Powerups
	Powerup power_index;

	vec2 startingJumpDir = vec2(0.0f, 0.0f);
	char startingJumpKey = 'N';
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
	

	float health;
	bool alive;
	float maxDepth;
	float maxFallTime;
	ColCube fallCube;
	vec3 fallCubeSize;
	float fallBoxTimer;
	bool scream;

	//running sound effect
	void PlayRunSoundEffect(float dt);
	float soundEffectCD = 0.4f;
	float currentSoundEffectCD = 0.f;
	std::string stepSounds[4];
	
public:
	void TakeDmg(int dmg = 1);
	void AddHealth(int hlt = 1);
	void AddScore(float scr = 1.0f);
	int GetHealth();
	float GetScore();
	bool IsAlive();
	void UpdateFallBox();
	GameObject* GOPTR; //GameObjectPlayerPointer//should not be here
};