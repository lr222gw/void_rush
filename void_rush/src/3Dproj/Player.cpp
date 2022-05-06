#include "Player.h"
#include <algorithm>

Player::Player(ModelObj* file, Graphics*& gfx, Camera*& cam, Mouse* mouse, Keyboard* keyboard, Hud* HUD, vec3 pos, vec3 rot, vec3 scale):
	GameObject(file, gfx, pos, rot, scale), noClip(false), HUD(HUD)
{
	this->mouse = mouse;
	this->keyboard = keyboard;
	this->cam = cam;
	this->gravity = vec3(0.0f, -9.82f, 0.0f);
	this->speed = vec3(2.7f, 0.0f, 2.7f);
	this->velocity = vec3(0.0f, 0.0f, 0.0f);
	this->jumpForce = 5.0f;
	this->midAirAdj = 2.0f;
	this->mass = 1.f;
	this->grounded = true;
	this->groundedTimer = 0.0f;
	this->shoved = false;

	this->power_index = EMPTY;

	GOPTR = static_cast<GameObject*>(this);
	setWeight(20);

	
	//setBoundingBox(DirectX::XMFLOAT3(0, -0.19, 0), DirectX::XMFLOAT3(0.19f, 0.10f, 0.19f));
	setBoundingBox(DirectX::XMFLOAT3(0, -scale.y - 0.01f, 0), DirectX::XMFLOAT3(scale.x - 0.01f, 0.10f, scale.z - 0.01f));
	this->health = 3;
	this->alive = true;
	this->maxDepth = -100.0f;
	this->resetGhost = false;
	this->submitName = false;

	this->maxLetters = 10;
	this->currentLetter = 0;
	for (int i = 0; i < maxLetters; i++) {
		this->name += "_";
	}
	this->scoreManager.SetPlayerSpeed(speed.length());

}

Player::~Player()
{
}

void Player::update(float dt)
{
	handleEvents(dt);

	if (!noClip) {
		scoreManager.Update(dt);
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
			if (velocity.length() > 0.3) {
				PlayRunSoundEffect(dt);
			}
		}
		if (this->groundedTimer != 0.0f)
		{
			this->groundedTimer += dt;
		}
		if (getPos().y < maxDepth) {
			TakeDmg();
		}
	}else{
		this->movePos(vec3(velocity.x * dt, 0.0f, velocity.z * dt));
	}


	this->setRot(vec3(0, cam->getRot().x, 0));
	cam->setPosition(this->getPos());

	if (this->velocity.y < -this->jumpForce) {
		sm->setSoundPosition("Wind", this->getPos());
		sm->setSoundVolume("Wind", abs(this->velocity.y + this->jumpForce) * 15);
	}
	else {
		sm->setSoundVolume("Wind", 0);
	}
	
	GameObject::update(dt);
}

void Player::handleEvents(float dt)
{
	vec2 airDir = vec2(0.0f, 0.0f);
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
		if (noClip)
		{
			translation = DirectX::XMFLOAT3(0, 0, -1);
			Translate(dt, translation);
		}
		if (!grounded)
		{
			if (this->startingJumpKey == 'W')
			{
				airDir = airDir + vec2(startingJumpDir.x, startingJumpDir.y);
			}
			else if (this->startingJumpKey == 'A')
			{
				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Upper left quadrant change to lower left quadrant. If (-0.5f, 0.5f) then change to (0.5, 0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower left quadrant change to lower right quadrant. If (-0.5f, -0.5f) then change to (-0.5, 0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower right quadrant change to top right quadrant. If (0.5f, -0.5f) then change to (-0.5, -0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Top right quadrant change to top left quadrant. If (0.5f, 0.5f) then change to (0.5, -0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}
			}
			else if (this->startingJumpKey == 'S')
			{
				airDir = airDir + vec2(-startingJumpDir.x, -startingJumpDir.y);
			}
			else if (this->startingJumpKey == 'D')
			{
				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Upper left quadrant change to lower left quadrant. If (-0.5f, 0.5f) then change to (-0.5, -0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower left quadrant change to lower right quadrant. If (-0.5f, -0.5f) then change to (0.5, -0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower right quadrant change to top right quadrant. If (0.5f, -0.5f) then change to (0.5, 0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Top right quadrant change to top left quadrant. If (0.5f, 0.5f) then change to (-0.5, 0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}
			}
		}
		else
		{
			startingJumpKey = 'W';
			cam->calcFURVectors();
			jumpDir = jumpDir + vec2(cam->getForwardVec().x, cam->getForwardVec().z);
		}

		if (this->startingJumpDir.legth() == 0.0f)
		{
			cam->calcFURVectors();
			airDir = airDir + vec2(cam->getForwardVec().x, cam->getForwardVec().z).Normalize();
		}
	}
	if (keyboard->isKeyPressed('D')) {
		if (noClip)
		{
			translation = DirectX::XMFLOAT3(-1, 0, 0);
			Translate(dt, translation);
		}
		if (!grounded)
		{
			if (this->startingJumpKey == 'W')
			{
				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Upper left quadrant change to lower left quadrant. If (-0.5f, 0.5f) then change to (0.5, 0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower left quadrant change to lower right quadrant. If (-0.5f, -0.5f) then change to (-0.5, 0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower right quadrant change to top right quadrant. If (0.5f, -0.5f) then change to (-0.5, -0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Top right quadrant change to top left quadrant. If (0.5f, 0.5f) then change to (0.5, -0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}
			}
			else if (this->startingJumpKey == 'A')
			{
				airDir = airDir + vec2(-startingJumpDir.x, -startingJumpDir.y);
			}
			else if (this->startingJumpKey == 'S')
			{
				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Upper left quadrant change to lower left quadrant. If (-0.5f, 0.5f) then change to (-0.5, -0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower left quadrant change to lower right quadrant. If (-0.5f, -0.5f) then change to (0.5, -0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower right quadrant change to top right quadrant. If (0.5f, -0.5f) then change to (0.5, 0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Top right quadrant change to top left quadrant. If (0.5f, 0.5f) then change to (-0.5, 0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}
			}
			else if (this->startingJumpKey == 'D')
			{
				airDir = airDir + vec2(startingJumpDir.x, startingJumpDir.y);
			}
		}
		else
		{
			startingJumpKey = 'D';
			cam->calcFURVectors();
			jumpDir = jumpDir + vec2(cam->getRightVector().x, cam->getRightVector().z);
		}

		if (this->startingJumpDir.legth() == 0.0f)
		{
			cam->calcFURVectors();
			airDir = airDir + vec2(cam->getRightVector().x, cam->getRightVector().z).Normalize();
		}
	}
	if (keyboard->isKeyPressed('S')) {
		if (noClip)
		{
			translation = DirectX::XMFLOAT3(0, 0, 1);
			Translate(dt, translation);
		}
		if (!grounded)
		{
			if (this->startingJumpKey == 'W')
			{
				airDir = airDir + vec2(-startingJumpDir.x, -startingJumpDir.y);
			}
			else if (this->startingJumpKey == 'A')
			{
				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Upper left quadrant change to lower left quadrant. If (-0.5f, 0.5f) then change to (-0.5, -0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower left quadrant change to lower right quadrant. If (-0.5f, -0.5f) then change to (0.5, -0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower right quadrant change to top right quadrant. If (0.5f, -0.5f) then change to (0.5, 0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Top right quadrant change to top left quadrant. If (0.5f, 0.5f) then change to (-0.5, 0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}
			}
			else if (this->startingJumpKey == 'S')
			{
				airDir = airDir + vec2(startingJumpDir.x, startingJumpDir.y);
			}
			else if (this->startingJumpKey == 'D')
			{
				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Upper left quadrant change to lower left quadrant. If (-0.5f, 0.5f) then change to (0.5, 0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower left quadrant change to lower right quadrant. If (-0.5f, -0.5f) then change to (-0.5, 0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower right quadrant change to top right quadrant. If (0.5f, -0.5f) then change to (-0.5, -0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Top right quadrant change to top left quadrant. If (0.5f, 0.5f) then change to (0.5, -0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}
			}
		}
		else
		{
			startingJumpKey = 'S';
			cam->calcFURVectors();
			jumpDir = jumpDir + vec2(-cam->getForwardVec().x, -cam->getForwardVec().z);
		}

		if (this->startingJumpDir.legth() == 0.0f)
		{
			cam->calcFURVectors();
			airDir = airDir + vec2(-cam->getForwardVec().x, -cam->getForwardVec().z).Normalize();
		}
	}
	if (keyboard->isKeyPressed('A')) {
		if (noClip)
		{
			translation = DirectX::XMFLOAT3(1, 0, 0);
			Translate(dt, translation);
		}
		if (!grounded)
		{
			if (this->startingJumpKey == 'W')
			{
				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Upper left quadrant change to lower left quadrant. If (-0.5f, 0.5f) then change to (-0.5, -0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower left quadrant change to lower right quadrant. If (-0.5f, -0.5f) then change to (0.5, -0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower right quadrant change to top right quadrant. If (0.5f, -0.5f) then change to (0.5, 0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Top right quadrant change to top left quadrant. If (0.5f, 0.5f) then change to (-0.5, 0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}
			}
			else if (this->startingJumpKey == 'A')
			{
				airDir = airDir + vec2(startingJumpDir.x, startingJumpDir.y);
			}
			else if (this->startingJumpKey == 'S')
			{
				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Upper left quadrant change to lower left quadrant. If (-0.5f, 0.5f) then change to (0.5, 0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x <= 0.0f && startingJumpDir.x >= -1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower left quadrant change to lower right quadrant. If (-0.5f, -0.5f) then change to (-0.5, 0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y <= 0.0f && startingJumpDir.y >= -1.0f)
				{
					//Lower right quadrant change to top right quadrant. If (0.5f, -0.5f) then change to (-0.5, -0.5f)
					airDir = airDir + vec2(-startingJumpDir.x, startingJumpDir.y);
				}

				if (startingJumpDir.x >= 0.0f && startingJumpDir.x <= 1.0f && startingJumpDir.y >= 0.0f && startingJumpDir.y <= 1.0f)
				{
					//Top right quadrant change to top left quadrant. If (0.5f, 0.5f) then change to (0.5, -0.5f)
					airDir = airDir + vec2(startingJumpDir.x, -startingJumpDir.y);
				}
			}
			else if (this->startingJumpKey == 'D')
			{
				airDir = airDir + vec2(-startingJumpDir.x, -startingJumpDir.y);
			}
		}
		else
		{
			startingJumpKey = 'A';
			cam->calcFURVectors();
			jumpDir = jumpDir + vec2(-cam->getRightVector().x, -cam->getRightVector().z);
		}

		if (this->startingJumpDir.legth() == 0.0f)
		{
			cam->calcFURVectors();
			airDir = airDir + vec2(-cam->getRightVector().x, -cam->getRightVector().z).Normalize();
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
			grounded = false;
			groundedTimer = 0.001f;
			startingJumpDir = jumpDir;
			if (startingJumpDir.legth() != 0.0f)
			{
				startingJumpDir.Normalize();
			}
			
			if (velocity.y > 0.0f)
			{
				velocity.y += jumpForce;
			}
			else
			{
				velocity = vec3(0.0f, jumpForce, 0.0f);
			}
			sm->playSound("Jump", getPos());
		}
		else {
			this->movePos(vec3(0.0f, speed.y * dt, 0.0f));
		}
	}
	if (keyboard->isKeyPressed(VK_CONTROL)) {
		this->movePos(vec3(0, -speed.y * dt, 0));
	}

	static vec3 rememberSpeed = this->speed;
	static bool held = false;
	static bool released = false;
	if (keyboard->isKeyPressed(VK_SHIFT)) {

		if (!held && released && grounded) {
			held = true;
			released = false;
			rememberSpeed = this->speed;
			//std::cout << "PRESSED \n";
			this->speed = this->speed * 2;
		}
		else if (held && !released) {
			//std::cout << "HELD \n";			
		}

	}
	else {
		//std::cout << "Released\n";
		if (!released) {
			this->speed = rememberSpeed;
		}
		held = false;
		released = true;
	}

	jumpDir.Normalize();
	if (grounded)
	{
		velocity.x = speed.x * jumpDir.x;
		velocity.z = speed.z * jumpDir.y;
	}
	else
	{
		if (!shoved)
		{
			if (!airDir.legth() == 0.0f)
			{
				airDir = airDir / vec2(midAirAdj, midAirAdj);
			}
			airDir = airDir + startingJumpDir;

			velocity.x = speed.x * airDir.x;
			velocity.z = speed.z * airDir.y;

		}
		else
		{
			velocity.x = shove.x;
			velocity.z = shove.y;
			
		}
	}
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
		float volume = (this->velocity.length() / this->speed.length())*15;

		this->grounded = true;
		this->shoved = false;
		this->velocity = vec3(0.0f, 0.0f, 0.0f);
		this->acceleration.y = 0.0f;
		this->resForce.y = 0.0f;
		this->groundedTimer = 0.0f;
		this->startingJumpDir = vec2(0.0f, 0.0f);
		this->startingJumpKey = 'N';

		sm->setSoundVolume("Land", volume);
		sm->playSound("Land", this->getPos());
	}
}

void Player::setUngrounded()
{
	if (grounded && !noClip)
	{
		this->grounded = false;
		this->startingJumpDir = jumpDir;
		groundedTimer = 0.001f;
	}
}

float Player::getSpeed()
{
	return this->speed.x;
}

float Player::getJumpForce()
{
	return this->jumpForce;
}

float Player::getGravity()
{
	return this->gravity.y;
}

float Player::getGroundedTimer()
{
	return this->groundedTimer;
}

GameObject*& Player::getPlayerObjPointer()
{
	return GOPTR;
}

void Player::Reset(bool lvlClr)
{
	resetGhost = true;

	this->shoved = false;
	this->grounded = true;
	this->resForce = vec3(0.0f, 0.0f, 0.0f);
	this->jumpDir = vec2(0.0f, 0.0f);
	this->acceleration = vec3(0.0f, 0.0f, 0.0f);
	this->velocity = vec3(0.0f, 0.0f, 0.0f);
	this->setPos(vec3(0.0f, 0.0f, 0.0f));
	this->groundedTimer = 0.0f;
	this->jumpDir = vec2(0.0f, 0.0f);
	this->startingJumpDir = vec2(0.0f, 0.0f);

	if (lvlClr) {
		//Add points
		scoreManager.LevelDone();
	}
}

bool Player::ResetGhost()
{
	if (resetGhost) {
		resetGhost = false;
		return true;
	}
	return false;
}

//Used by enemies to move player on collision
void Player::shovePlayer(vec2 shove, float forceY)
{
	this->groundedTimer = 0.001f;
	this->grounded = false;
	this->shoved = true;
	this->shove = shove;
	this->velocity.y = forceY;
	sm->playSound("Hit", getPos());
	sm->playSound("Shoved", getPos());
	ResetGhost();
}

//gets the powerup index from collission handler when one is picked up
void Player::pickedUpPower(Powerup index)
{
	sm->playSound("Pickup", getPos());
	this->power_index = index;
	if (this->power_index == FEATHER)
	{
		this->HUD->TurnOnPassive(FEATHER_P);
	}
	if (this->power_index == PEARL)
	{
		this->HUD->TurnOnPassive(PEARL_P);
	}
	if (this->power_index == POTION)
	{
		this->HUD->TurnOnPassive(POTION_P);
	}
	if (this->power_index == SHIELD)
	{
		this->HUD->TurnOnPassive(SHIELD_P);
	}
	else
	{
		this->HUD->ChangeCurrentPowerUp(this->power_index);
	}

	if (this->power_index == APPLE)
	{
		HUD->IncreaseHealth();
	}
	
}

Powerup Player::getPlayerPower()
{
	return this->power_index;
}

void Player::setPlayerPower(Powerup index)
{
	this->power_index = index;
}


//void Player::SetPuzzlePos(vec3 puzzlePosition)
void Player::SetDifficulity(Difficulity diff)
{
	scoreManager.SetDifficulty(diff);
}

void Player::SetStartPlatform(Platform*& start)
{
	scoreManager.SetStartPlatform(start);
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

void Player::writeScore(std::string file)
{
	scoreManager.WriteScore(name, file);
}

void Player::AddToName(unsigned char letter)
{
	if (currentLetter < maxLetters) {
		name.at(currentLetter++) = letter;
	}
}

void Player::RemoveLetter()
{
	if (currentLetter > 0) {
		name.at(--currentLetter) = '_';
	}
	else {
		name.at(0) = '_';
	}
}

std::string Player::GetName() const
{
	return name;
}

int Player::GetMaxLetters()
{
	return maxLetters;
}

int Player::GetCurrentLetter()
{
	return currentLetter;
}

void Player::ResetName()
{
	for (int i = 0; i < maxLetters; i++) {
		this->name.at(i) = '_';
	}
}

bool Player::GetSubmitName()
{
	return submitName;
}

void Player::SetSubmitName(bool val)
{
	submitName = val;
}

void Player::SetCurrentSeed(int seed)
{
	scoreManager.SetSeed(seed);
}

void Player::PlayRunSoundEffect(float dt)
{
	static int a = 0;
	currentSoundEffectCD -= dt;
	if (currentSoundEffectCD < 0 && sm != nullptr) {
		currentSoundEffectCD = soundEffectCD;
		sm->playSound(stepSounds[a % 4], getPos());
		a++;
		//sm->playSound("Goat", getPos());
	}
}

void Player::getSoundManager(SoundManager& sm)
{
	//set sounds
	stepSounds[0] = "step1";
	stepSounds[1] = "step2";
	stepSounds[2] = "step3";
	stepSounds[3] = "step4";
	for (int i = 0; i < 4; i++) {
		sm.loadSound("assets/audio/" + stepSounds[i] + ".ogg", 90, stepSounds[i]);
	}

	GameObject::getSoundManager(sm);
}

SoundManager* Player::getSm() const
{
	return sm;
}

void Player::TakeDmg(int dmg)
{
	health-=dmg;
	this->Reset();	
	if(health <= 0 && !this->invincible) {
        alive = false;
		sm->playSound("GameOver", getPos());
	}
	else {
		scoreManager.setDamageScore();
		sm->playSound("Scream", getPos());
	}
	this->HUD->LowerHealth();
}

void Player::AddHealth(int hlt)
{
	health += hlt;
}

void Player::AddScore(float scr)
{
	this->scoreManager.AddScore(scr);
}

int Player::GetHealth()
{
	return health;
}

float Player::GetScore()
{
	return scoreManager.GetScore();
}

bool Player::IsAlive()
{
	return alive;
}