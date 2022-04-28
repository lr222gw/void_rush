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
	float getGroundedTimer();
	GameObject*& getPlayerObjPointer();
	void Reset(bool lvlClr = false);
	bool ResetGhost();

	void SetDifficulity(Difficulity diff);
	void SetStartPlatform(Platform*& start);
	void writeScore(std::string name, std::string file = "assets/files/highScores.txt");

	
private:
	friend class ImguiManager;
	bool noClip;
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

	bool resetGhost;
	vec2 startingJumpDir = vec2(0.0f, 0.0f);
	char startingJumpKey = 'N';
	bool isKeyPressed = false;

	Mouse* mouse;
	Keyboard* keyboard;
	Camera* cam;

	ScoreManager scoreManager;
	float levelTime;
	vec3 puzzlePos;
	Difficulity levelDifficulty;
	Hud* HUD;

	int health;
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