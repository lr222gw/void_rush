#pragma once
#include "GameObject.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Camera.h"

class Player : public GameObject {
public:
	Player(ModelObj* file, Graphics*& gfx, Camera*& cam, Mouse* mouse, Keyboard* keyboard, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	virtual ~Player();
	void update(float dt);
	void handleEvents(float dt);
	void rotateWithMouse(int x, int y);
	void addRot(vec3 rot);
private:
	void Translate(float dt, DirectX::XMFLOAT3 translate);
	vec3 velocity;
	Mouse* mouse;
	Keyboard* keyboard;
	Camera* cam;
	float speed;

	/*New*/
	int health;
	bool alive;
public:
	void TakeDmg(int dmg = 1);
	void AddHealth(int hlt = 1);
	int GetHealth();
	bool IsAlive();
};