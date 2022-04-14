#include "Player.h"

Player::Player(ModelObj* file, Graphics*& gfx, Camera*& cam, Mouse* mouse, Keyboard* keyboard, vec3 pos, vec3 rot, vec3 scale):
	GameObject(file, gfx, pos, rot, scale)
{
	this->mouse = mouse;
	this->keyboard = keyboard;
	this->cam = cam;
	this->gravity = vec3(0.0f, -9.82f, 0.0f);
	this->speed = 5.f;
	this->jumpSpeed = 1.f;
	this->velocity = vec3(0.0f, 0.0f, 0.0f);
	this->mass = 1.f;
	this->grounded = true;
	GOPTR = static_cast<GameObject*>(this);
	setWeight(20);
}

Player::~Player()
{
}

void Player::update(float dt)
{
	handleEvents(dt);
	cam->setRotation(vec3(this->getRot().y, -this->getRot().x, this->getRot().z));
	if (!grounded)
	{
		// Update forces
		resForce = gravity * mass;

		// Update acceleration
		acceleration = resForce / mass;
		// Update velocity
		velocity = velocity + acceleration * dt;
	}
	this->movePos(vec3(0, this->velocity.y * dt, 0));
	cam->setRotation(this->getRot());
	cam->setPosition(this->getPos());
}

void Player::handleEvents(float dt)
{
	//change these to use keyboard
	if (!mouse->getMouseActive()) {
		if (GetKeyState(VK_RIGHT) & 0x8000) {
			this->addRot(vec3(0, mouse->getSense() * (float)dt, 0));
		}
		if (GetKeyState(VK_LEFT) & 0x8000) {
			this->addRot(vec3(0, -mouse->getSense() * (float)dt, 0));
		}
		if (GetKeyState(VK_UP) & 0x8000) {
			this->addRot(vec3(-mouse->getSense() * (float)dt, 0, 0));
		}
		if (GetKeyState(VK_DOWN) & 0x8000) {
			this->addRot(vec3(mouse->getSense() * (float)dt, 0, 0));
		}
	}
	//change values here
	DirectX::XMFLOAT3 translation = DirectX::XMFLOAT3(0, 0, 0);
	if (keyboard->isKeyPressed('W')) {
		translation = DirectX::XMFLOAT3(0, 0, -1);
		Translate(dt, translation);
	}
	if (keyboard->isKeyPressed('D')) {
		translation = DirectX::XMFLOAT3(-1, 0, 0);
		Translate(dt, translation);
	}
	if (keyboard->isKeyPressed('S')) {
		translation = DirectX::XMFLOAT3(0, 0, 1);
		Translate(dt, translation);
	}
	if (keyboard->isKeyPressed('A')) {
		translation = DirectX::XMFLOAT3(1, 0, 0);
		Translate(dt, translation);
	}
	if (keyboard->isKeyPressed(VK_SPACE)) {
		if (grounded) {
			grounded = false;
		}
		if (velocity.y > 0)
		{
			velocity.y += jumpSpeed;
		}
		else
		{
			velocity = vec3(velocity.x, jumpSpeed, velocity.z);
		}
		//this->movePos(vec3(0, this->velocity.y * dt, 0));
	}
	if (keyboard->isKeyPressed(VK_CONTROL)) {
		this->movePos(vec3(0, -speed * dt, 0));
	}

}

void Player::rotateWithMouse(int x, int y)
{
	this->addRot(vec3(
		static_cast<float>(y) * mouse->getSense() * 0.001,
		static_cast<float>(x)* mouse->getSense() * 0.001,
		0
	));
}

void Player::addRot(vec3 rot)
{
	
	if (this->getRot().y + rot.y < -1.5 || this->getRot().y + rot.y > 1.57f)
		rot.y = 0;
	object::addRot(rot);
}

GameObject*& Player::getPlayerObjPointer()
{
	return GOPTR;
}

void Player::Translate(float dt, DirectX::XMFLOAT3 translate)
{
	DirectX::XMStoreFloat3(&translate, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translate),
		DirectX::XMMatrixRotationRollPitchYaw(this->getxRot(), this->getyRot(), 0) *
		DirectX::XMMatrixScaling(1, 1, 1)//this line is not neccessary but I am afraid to break things
	));
	vec2 trans(translate.x, translate.z);
	trans.Normalize();
	this->setPos(vec3(
		this->getPos().x - trans.x * speed * dt,
		this->getPos().y,
		this->getPos().z - trans.y * speed * dt
	));
}
