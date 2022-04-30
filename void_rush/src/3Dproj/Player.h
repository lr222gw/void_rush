#pragma once
#include "GameObject.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Camera.h"

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
	float getGroundedTimer();
	GameObject*& getPlayerObjPointer();
	void Reset();
	
private:
	friend class ImguiManager;
	bool noClip;
	bool invincible;
	void Translate(float dt, DirectX::XMFLOAT3 translate);
	vec3 speed;
	vec3 jumpSpeed;
	vec3 velocity;
	vec3 acceleration;
	vec3 resForce;
	vec3 gravity;
	vec2 jumpDir;
	float mass;
	bool grounded;
	float groundedTimer;


	Mouse* mouse;
	Keyboard* keyboard;
	Camera* cam;

	int health;
	bool alive;
	float maxDepth;
public:
	void TakeDmg(int dmg = 1);
	void AddHealth(int hlt = 1);
	int GetHealth();
	bool IsAlive();
	GameObject* GOPTR;//GameObjectPlayerPointer
};