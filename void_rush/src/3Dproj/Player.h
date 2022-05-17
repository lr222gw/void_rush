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
#include "powerups/PowerUpDefs.hpp"

#include <string>
#include <fstream>

enum Powerup;

//used with HUD
enum PowerUpPassiv
{
	EMPTY_P,
	FEATHER_P,
	PEARL_P,
	POTION_P,
	SHIELD_P,
	APPLE_P,
	MONEY_P
};

class Player : public GameObject {
public:
	Player(ModelObj* file, Graphics*& gfx, Camera*& cam, Mouse* mouse, Keyboard * keyboard, Hud* HUD, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	virtual ~Player();
	void update(float dt) override;
	void handleEvents(float dt);
	void rotateWithMouse(int x, int y);
	void addRot(vec3 rot);
	void setGrounded();
	void setUngrounded();
	float getSpeed();
	bool isGrounded();
	float getJumpForce();
	float getGravity();
	float getGroundedTimer();
	GameObject*& getPlayerObjPointer();
	ColCube getFallCube()const;
	void ResetFallBoxTimer();
	void Reset(bool lvlClr = false);

	void lookat(vec3 lookat, vec3 offset = vec3(0,0,0));

	//Used when player falls of platform to rest ghost
	bool ResetGhost();
	void shovePlayer(vec2 shove, float forceY);
	void bouncePlayer(vec2 bounceVec, float forceY);
	void setVelocity(vec3 vel);
	vec3 getVelocity()const;

	//Powerup function
	void pickedUpPower(Powerup index);
	Powerup getPlayerPower();
	PowerUpPassiv getPassivePower();
	void setPlayerPower(Powerup index);
	void setPlayerPowerPassiv(PowerUpPassiv index);
	void setCanDoubleJump();
	bool canDoubleJump();
	void getShield();
	void setPlayerSpeed(vec3 speed);
	void useRocket(bool trueOrFalse);

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
	void setMusicVol(float vol);

	bool isInvinc()const;

	void set_resetLookat_dir(vec3 lookAt);
	
	void SetPearl(GameObject*& pearlObj);
	void MovePearl(vec3 pos);
	void SetPearlPos(vec3 pos);
	void PearlHit();
	bool getPearlStatus();
	void setPearlStatus(bool trueOrFalse);
	void resetPearl();
	GameObject*& GetPearl();
	Keyboard* GetKB();

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
	vec3 resetLookat_dir; 
	float mass;
	bool grounded;
	float groundedTimer;
	GameObject* pearl;
	bool pearlActive = false;

	//For being shoved
	bool shoved;
	vec2 shove;
	bool shoveDelay;
	float shoveTimer;
	//For being bounced
	bool bounced;
	vec2 bounceVec;
	//Beat
	float heartBeatTimer;
	float bpm;
	float musicVol;

	//Shake
	bool screenShake;

	//Powerups
	Powerup power_index;
	PowerUpPassiv passive_powerup;
	bool canDoublejump;
	bool hasShield;
	bool usingRocket;
	float storeSpeed;

	vec2 startingJumpDir = vec2(0.0f, 0.0f);
	char startingJumpKey = 'N';
	bool resetGhost;

	Mouse* mouse;
	Keyboard* keyboard;
	Camera* cam;
	Graphics*& gfx;

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

	float maxFOV;
	float currentFOV;
	float minFOV;

	//running sound effect
	void PlayRunSoundEffect(float dt);
	float soundEffectCD = 0.4f;
	float currentSoundEffectCD = 0.f;
	std::string stepSounds[4];

	//Used for falling platforms
	bool hold;
	
public:
	void TakeDmg(int dmg = 1);
	void AddHealth(int hlt = 1);
	void AddScore(float scr = 1.0f);
	int GetHealth();
	float GetScore();
	vec3 GetForwardVec();
	bool IsAlive();
	void UpdateFallBox();
	GameObject* GOPTR; //GameObjectPlayerPointer//should not be here
};