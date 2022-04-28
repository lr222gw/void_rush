#include "Player.h"
#include <algorithm>

Player::Player(ModelObj* file, Graphics*& gfx, Camera*& cam, Mouse* mouse, Keyboard* keyboard, vec3 pos, vec3 rot, vec3 scale):
	GameObject(file, gfx, pos, rot, scale), noClip(true)
{
	this->mouse = mouse;
	this->keyboard = keyboard;
	this->cam = cam;
	this->gravity = vec3(0.0f, -9.82f, 0.0f);
	this->speed = vec3(5.f, 5.0f, 5.0f);
	this->jumpSpeed = vec3(0.0f, 0.0f, 0.0f);
	this->velocity = vec3(0.0f, 0.0f, 0.0f);
	this->mass = 1.f;
	this->grounded = true;
	this->groundedTimer = 0.0f;
	GOPTR = static_cast<GameObject*>(this);
	this->setScale(vec3(0.2f,0.2f,0.2f));
	setWeight(20);
	setBoundingBox(DirectX::XMFLOAT3(0, -0.9f, 0), DirectX::XMFLOAT3(0.3f, 0.5f, 0.3f));
	this->health = 3;
	this->alive = true;
	this->maxDepth = -140.0f;
}

Player::~Player()
{
}

void Player::update(float dt)
{
	handleEvents(dt);
	if (!noClip) {
		if (!grounded)
		{
			// Update forces
			resForce = gravity * mass;
			// Update acceleration
			acceleration = resForce / mass;
			// Update velocity
			velocity = velocity + acceleration * dt;
			this->movePos(vec3(velocity.x * dt, velocity.y * dt, (velocity.z) * dt));
		}
		if (grounded)
		{
			this->movePos(vec3(velocity.x* dt, 0.0f, velocity.z * dt));
		}
		if (this->groundedTimer != 0.0f)
		{
			this->groundedTimer += dt;
		}
		if (getPos().y < maxDepth) {
			TakeDmg();
			Reset();
		}
	}
	//std::cout << "speed.y: " << speed.y << " Velocity.y: " << velocity.y << std::endl;
	this->setRot(vec3(0, cam->getRot().x, 0));
	cam->setPosition(this->getPos());
	GameObject::update(dt);
}

void Player::handleEvents(float dt)
{
	//change these to use keyboard
	if (!mouse->getMouseActive()) {
		if (GetKeyState(VK_RIGHT) & 0x8000) {
			cam->addRotation(vec3(mouse->getSense() * (float)dt, 0, 0));
		}
		if (GetKeyState(VK_LEFT) & 0x8000) {
			cam->addRotation(vec3(-mouse->getSense() * (float)dt, 0, 0));
		}
		if (GetKeyState(VK_UP) & 0x8000) {
			cam->addRotation(vec3(0, mouse->getSense() * (float)dt, 0));
		}
		if (GetKeyState(VK_DOWN) & 0x8000) {
			cam->addRotation(vec3(0, -mouse->getSense() * (float)dt, 0));
		}
	}
	//change values here
	DirectX::XMFLOAT3 translation = DirectX::XMFLOAT3(0, 0, 0);
	if (keyboard->isKeyPressed('W')) {
		cam->calcFURVectors();
		jumpDir = jumpDir + vec2(cam->getForwardVec().x,  cam->getForwardVec().z);
		
		if (/*grounded ||*/ noClip)
		{
			translation = DirectX::XMFLOAT3(0, 0, -1);
			Translate(dt, translation);
		}
		if (!grounded)
		{

		}
	}
	if (keyboard->isKeyPressed('D')) {
		cam->calcFURVectors();
		jumpDir = jumpDir + vec2(cam->getRightVector().x, cam->getRightVector().z);
		
		if (/*grounded ||*/ noClip)
		{
			translation = DirectX::XMFLOAT3(-1, 0, 0);
			Translate(dt, translation);
		}
		if (!grounded)
		{

		}
	}
	if (keyboard->isKeyPressed('S')) {
		cam->calcFURVectors();
		jumpDir = jumpDir + vec2(-cam->getForwardVec().x,  -cam->getForwardVec().z);
		
		if (/*grounded ||*/ noClip)
		{
			translation = DirectX::XMFLOAT3(0, 0, 1);
			Translate(dt, translation);
		}
		if (!grounded)
		{
			
		}
	}
	if (keyboard->isKeyPressed('A')) {
		cam->calcFURVectors();
		jumpDir = jumpDir + vec2(-cam->getRightVector().x, -cam->getRightVector().z);
		
		if (/*grounded ||*/ noClip)
		{
			translation = DirectX::XMFLOAT3(1, 0, 0);
			Translate(dt, translation);
		}
		if (!grounded)
		{
			
		}
	}
	if (!keyboard->isKeyPressed('W') && !keyboard->isKeyPressed('A') && !keyboard->isKeyPressed('S') && !keyboard->isKeyPressed('D'))
	{
		if (grounded)
		{
			jumpDir = vec2(0.0f, 0.0f);
		}
	}
	if (keyboard->isKeyPressed(VK_SPACE) && grounded) {
		if(!noClip){
			if (grounded) {
				grounded = false;
				groundedTimer = 0.001f;
			}
			if (velocity.y > 0.0f)
			{
				velocity.y += speed.y;
			}
			else
			{
				velocity = vec3(0.0f, speed.y, 0.0f);
			}
		}
		else {
			this->movePos(vec3(0.0f, speed.y * dt, 0.0f));
		}
	}
	if (keyboard->isKeyPressed(VK_CONTROL)) {
		this->movePos(vec3(0, -speed.y * dt, 0));
	}
	jumpDir.Normalize();
	//jumpSpeed = vec3(speed.x * jumpDir.x, speed.y, speed.z * jumpDir.y);
	velocity.x = speed.x * jumpDir.x;
	velocity.z = speed.z * jumpDir.y;
}

void Player::rotateWithMouse(int x, int y)
{
	float ycr = cam->getRot().y + static_cast<float>(y) * -mouse->getSense() * 0.01;
	ycr = std::clamp(ycr, -1.5f, 1.57f);

	cam->setRotation(vec3(
		cam->getRot().x + static_cast<float>(x) * mouse->getSense() * 0.01,
		ycr,
		0
	));
}

void Player::addRot(vec3 rot)
{
	
	if (this->getRot().y + rot.y < -1.5 || this->getRot().y + rot.y > 1.57f)
		rot.y = 0;
	object::addRot(rot);
}

void Player::setGrounded()
{
	if (!grounded)
	{
		this->grounded = true;
		this->velocity = vec3(0.0f, 0.0f, 0.0f);
		this->acceleration.y = 0.0f;
		this->resForce.y = 0.0f;
		this->groundedTimer = 0.0f;
		//this->speed = vec3(5.0f, 5.0f, 5.0f);
	}
}

void Player::setUngrounded()
{
	if (grounded && !noClip)
	{
		this->grounded = false;
		groundedTimer = 0.001f;
	}
}

float Player::getGroundedTimer()
{
	return this->groundedTimer;
}

GameObject*& Player::getPlayerObjPointer()
{
	return GOPTR;
}

void Player::Reset()
{
	this->grounded = true;
	this->setPos(vec3(0.0f, 0.0f, 0.0f));
	this->velocity = vec3(0.0f, 0.0f, 0.0f);
	this->acceleration = vec3(0.0f, 0.0f, 0.0f);
	this->resForce = vec3(0.0f, 0.0f, 0.0f);
	this->groundedTimer = 0.0f;
}


void Player::Translate(float dt, DirectX::XMFLOAT3 translate)
{
	DirectX::XMStoreFloat3(&translate, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translate),
		DirectX::XMMatrixRotationRollPitchYaw(cam->getRot().y, cam->getRot().x, 0) *
		DirectX::XMMatrixScaling(1, 1, 1)//this line is not neccessary but I am afraid to break things
	));
	vec2 trans(translate.x, translate.z);
	trans.Normalize();
	this->setPos(vec3(
		this->getPos().x - trans.x * speed.x * dt,
		this->getPos().y,
		this->getPos().z - trans.y * speed.z * dt
	));
}


void Player::TakeDmg(int dmg)
{
	health-=dmg;
	if(health <= 0) {
		//alive = false; //TODO: ÄNDRA TILLBAKA! 
	}
}

void Player::AddHealth(int hlt)
{
	health += hlt;
}

int Player::GetHealth()
{
	return health;
}

bool Player::IsAlive()
{
	return alive;
}
