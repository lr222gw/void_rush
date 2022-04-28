#pragma once
#include "GameObject.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Camera.h"
#include "common/Vector.hpp"

#include "generation/Position_generator.hpp"//To use difficulty

#include <string>
#include <fstream>

class Player : public GameObject {
public:
	Player(ModelObj* file, Graphics*& gfx, Camera*& cam, Mouse* mouse, Keyboard* keyboard, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
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

	void SetPuzzlePos(vec3 puzzlePosition);
	void SetDifficulity(Difficulity diff);

	//score -1: using players score
	void writeScore(std::string name, float score = -1, std::string file = "assets/files/highScores.txt");

	
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

	vec2 startingJumpDir = vec2(0.0f, 0.0f);
	char startingJumpKey = 'N';
	bool isKeyPressed = false;
	bool resetGhost;

	Mouse* mouse;
	Keyboard* keyboard;
	Camera* cam;

	float levelTime;
	vec3 puzzlePos;
	Difficulity levelDifficulty;

	float score;
	float health;
	bool alive;
	float maxDepth;

	const float constPoints = 0.1f;//Points given each update;
	const float puzzlePoints = 100.0f;//Points given when puzzle is done
	const float levelPoints = 1000.0f;//Points given when level is done
	const float deathPoints = -5.0f;//Points given when player looses a life
	//const std::string scoreFile = "asstes/files/highScores.txt";

	const int maxScores = 10;
	
public:
	void TakeDmg(int dmg = 1);
	void AddHealth(int hlt = 1);
	void AddScore(float scr = 1.0f);
	int GetHealth();
	float GetScore();
	bool IsAlive();
	GameObject* GOPTR; //GameObjectPlayerPointer
};