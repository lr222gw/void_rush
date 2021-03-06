#include "Player.h"
#include <algorithm>

Player::Player(ModelObj* file, Graphics*& gfx, Camera*& cam, Mouse* mouse, Keyboard* keyboard, Hud* HUD, vec3 pos, vec3 rot, vec3 scale):
	GameObject(file, gfx, pos, rot, scale), noClip(false), HUD(HUD), gfx(gfx)
{
	this->mouse = mouse;
	this->keyboard = keyboard;
	this->cam = cam;
	this->gravity = vec3(0.0f, -15.f, 0.0f);
	this->baseSpeed = vec3(5.3f, 0.0f, 5.3f);
	this->speed = this->baseSpeed;
	this->storeSpeed = this->speed.x;
	this->velocity = vec3(0.0f, 0.0f, 0.0f);
	this->jumpForce = 8.0f;
	this->midAirAdj = 2.0f;
	this->mass = 1.f;
	this->grounded = true;
	this->groundedTimer = 0.0f;
	this->shoved = false;

	this->power_index = EMPTY;
	this->passive_powerup = EMPTY_P;
	this->canDoublejump = false;
	this->hasShield = false;

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
	this->scoreManager.SetPlayerSpeed(this->baseSpeed.length());
	this->scoreManager.SetHUD(HUD);
	fallCubeSize = vec3(50.0f, 200.0f, 50.0f);
	UpdateFallBox();
	fallBoxTimer = 0.0f;
	scream = false;
	this->shoveDelay = false;
	this->shoveTimer = 0.0f;
	this->heardBeat = true;
	this->heartBeatTimer = 0.0f;
	this->bpm = 60;
	this->musicVol = 0.0f;

	currentFOV = 45;
	minFOV = 45;

	screenShake = true;
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
		if (fallBoxTimer > 2 && !scream) {
			sm->playSound("Scream", getPos());
			scream = true;
		}
		if (fallBoxTimer > 4) {
			TakeDmg();
		}
		if (velocity.length() > 0) {
			currentFOV = lerp(currentFOV, maxFOV, 1 * dt);
		}
		else {
			currentFOV = lerp(currentFOV, minFOV, 1 * dt);
		}
		gfx->setFov(currentFOV);
		fallBoxTimer+=dt;
	}else{
		this->movePos(vec3(velocity.x * dt, 0.0f, velocity.z * dt));
	}
	UpdateFallBox();

	heartBeatTimer += dt;

	this->setRot(vec3(0, cam->getRot().x, 0));
	cam->setPosition(this->getPos());

	if (this->velocity.y < -this->jumpForce) {
		sm->setSoundPosition("Wind", this->getPos());
		sm->setSoundVolume("Wind", abs(this->velocity.y + this->jumpForce) * 15);
	}
	else {
		sm->setSoundVolume("Wind", 0);
	}
	if (shoveDelay) {
		shoveTimer += dt;
		if (shoveTimer > 0.1f) {
			shoveDelay = false;
			shoveTimer = 0.0f;
			sm->playSound("Shoved", getPos());
		}
	}
	if (heartBeatTimer >= 60/bpm) {
		if (heardBeat) {
			sm->setSoundVolume("HeartBeat", 30 + 30/(260 / bpm));
			sm->playSound("HeartBeat", getPos());
		}
		heartBeatTimer = 0.0f;
	}
	sm->setSoundVolume("MusicChange", musicVol);
	
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
				airDir = airDir + vec2(startingJumpDir.y, -startingJumpDir.x);
			}
			else if (this->startingJumpKey == 'S')
			{
				airDir = airDir + vec2(-startingJumpDir.x, -startingJumpDir.y);
			}
			else if (this->startingJumpKey == 'D')
			{
				airDir = airDir + vec2(-startingJumpDir.y, startingJumpDir.x);
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
				airDir = airDir + vec2(startingJumpDir.y, -startingJumpDir.x);
			}
			else if (this->startingJumpKey == 'A')
			{
				airDir = airDir + vec2(-startingJumpDir.x, -startingJumpDir.y);
			}
			else if (this->startingJumpKey == 'S')
			{
				airDir = airDir + vec2(-startingJumpDir.y, startingJumpDir.x);
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
				airDir = airDir + vec2(-startingJumpDir.y, startingJumpDir.x);
			}
			else if (this->startingJumpKey == 'S')
			{
				airDir = airDir + vec2(startingJumpDir.x, startingJumpDir.y);
			}
			else if (this->startingJumpKey == 'D')
			{
				airDir = airDir + vec2(startingJumpDir.y, -startingJumpDir.x);
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
				airDir = airDir + vec2(-startingJumpDir.y, startingJumpDir.x);
			}
			else if (this->startingJumpKey == 'A')
			{
				airDir = airDir + vec2(startingJumpDir.x, startingJumpDir.y);
			}
			else if (this->startingJumpKey == 'S')
			{
				airDir = airDir + vec2(startingJumpDir.y, -startingJumpDir.x);
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
	if ((keyboard->isKeyPressed(VK_SPACE) && (grounded || jumpAfterPlatformTimer > 0.0f || canDoublejump))) {
		if(!noClip){
			if (jumpAfterPlatformTimer > 0.0f)
			{
				jumpAfterPlatformTimer = 0.0f;
			}

			if (canDoublejump == true && grounded)
			{
				this->canDoublejump = false;
			}
			grounded = false;
			groundedTimer = 0.001f;
			startingJumpDir = jumpDir;
			if (startingJumpDir.legth() != 0.0f)
			{
				startingJumpDir.Normalize();
			}
			if (velocity.y > 0.0f)
			{
				velocity.y = jumpForce;
				if (canDoublejump == true)
				{
					sm->playSound("Feather");
					this->canDoublejump = false;
					this->HUD->TurnOffPassive(FEATHER_P);
				}
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
			this->speed = this->baseSpeed * 2;
		}
		else if (held && !released) {
			//std::cout << "HELD \n";			
		}
		maxFOV = 49;
	}
	else {
		//std::cout << "Released\n";
		if (!released) {
			this->speed = rememberSpeed;
		}
		held = false;
		released = true;
		maxFOV = 47;
	}

	static vec3 rememberGrav = this->gravity;
	static bool heldCtrl = false;
	static bool releasedCtrl = false;
	if (keyboard->isKeyPressed(VK_CONTROL)) {

		if (!heldCtrl && releasedCtrl && !grounded) {
			heldCtrl = true;
			releasedCtrl = false;
			rememberGrav = this->gravity;
			//std::cout << "PRESSED \n";
			this->gravity = this->gravity * 3;
		}
		else if (heldCtrl && !releasedCtrl) {
			//std::cout << "HELD \n";			
		}

	}
	else {
		//std::cout << "Released\n";
		if (!releasedCtrl) {
			this->gravity = rememberGrav;
		}
		heldCtrl = false;
		releasedCtrl = true;
	}

	jumpDir.Normalize();
	if (grounded)
	{
		velocity.x = speed.x * jumpDir.x;
		velocity.z = speed.z * jumpDir.y;
	}
	else
	{
		if (!shoved && !bounced && !usingRocket)
		{
			if (!airDir.legth() == 0.0f)
			{
				airDir = airDir / vec2(midAirAdj, midAirAdj);
			}

			airDir = airDir + startingJumpDir;

			if (startingJumpDir.legth() == 0.0f)
			{
				airDir = airDir * 1.5f;
			}

			velocity.x = speed.x * airDir.x;
			velocity.z = speed.z * airDir.y;

		}
		else if (usingRocket)
		{
			this->velocity = vec3(cam->getForwardVec().x, cam->getForwardVec().y, cam->getForwardVec().z) * 15.f;
		}
		else if (shoved)
		{
			velocity.x = shove.x;
			velocity.z = shove.y;
			
		}
		else if (bounced)
		{
			if (!airDir.legth() == 0.0f)
			{
				airDir = airDir / vec2(midAirAdj, midAirAdj);
			}
			airDir = airDir + startingJumpDir;

			velocity.x = speed.x  * airDir.x * 2.0f;
			velocity.z = speed.z  * airDir.y * 2.0f;

			velocity.x += bounceVec.x;
			velocity.z += bounceVec.y;


			float reduction = 2.f;
			if (bounceVec.x > 0.1f) {
				bounceVec.x -= reduction * dt;
			}
			else if(bounceVec.x < -0.1f) {
				bounceVec.x += reduction * dt;
			}
			else {
				bounceVec.x = 0.0f;
			}
			if (bounceVec.y > 0.1f) {
				bounceVec.y -= reduction * dt;
			}
			else if (bounceVec.y < -0.1f) {
				bounceVec.y += reduction * dt;
			}
			else {
				bounceVec.y = 0.0f;
			}
			
		}
	}
	
	if (this->jumpAfterPlatformTimer > 0.0f)
	{
		this->jumpAfterPlatformTimer -= dt;
		if (this->jumpAfterPlatformTimer <= 0.0f)
		{
			this->jumpAfterPlatformTimer = 0.0f;
		}
	}

	if (this->rocketTimer > 0.0f)
	{
		this->rocketTimer -= dt;
		if (this->rocketTimer <= 0.0f)
		{
			this->rocketTimer = 0.0f;
		}
	}
}

void Player::rotateWithMouse(int x, int y)
{
	float ycr = (float)(cam->getRot().y + static_cast<float>(y) * -mouse->getSense() * 0.01);
	ycr = std::clamp(ycr, -1.5f, 1.57f);

	cam->setRotation(vec3(
		(float)(cam->getRot().x + static_cast<float>(x) * mouse->getSense() * 0.01),
		(float)ycr,
		0.0f
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
	if (!grounded && rocketTimer == 0.0f)
	{

		float volume = (this->velocity.length() / this->speed.length())*15;
		if(screenShake)
			this->cam->screenShake(volume /20.0f);

		this->grounded = true;
		this->bounced = false;
		this->shoved = false;
		this->velocity = vec3(0.0f, 0.0f, 0.0f);
		this->acceleration.y = 0.0f;
		this->resForce.y = 0.0f;
		this->groundedTimer = 0.0f;
		this->startingJumpDir = vec2(0.0f, 0.0f);
		this->startingJumpKey = 'N';
		this->fallBoxTimer = 0.0f;
		this->scream = false;
		this->jumpAfterPlatformTimer = 0.0f;

		sm->setSoundVolume("Land", volume);
		sm->playSound("Land", this->getPos());
		
	}
}

void Player::setUngrounded()
{
	if (grounded && !noClip)
	{
		this->grounded = false;
		this->startingJumpDir = jumpDir / 2;
		groundedTimer = 0.001f;
		this->jumpAfterPlatformTimer = 0.15f;
	}
}

float Player::getSpeed()
{
	return this->speed.x;
}

float Player::getBaseSpeed()
{
	return this->baseSpeed.x;
}


bool Player::isGrounded()
{
	return this->grounded;
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

ColCube Player::getFallCube() const
{
	return fallCube;
}

void Player::ResetFallBoxTimer()
{
	fallBoxTimer = 0.0f;
	this->scream = false;
}

void Player::Reset(bool lvlClr)
{
	resetGhost = true;

	this->shoved = false;
	this->bounced = false;
	this->grounded = true;
	this->resForce = vec3(0.0f, 0.0f, 0.0f);
	this->jumpDir = vec2(0.0f, 0.0f);
	this->acceleration = vec3(0.0f, 0.0f, 0.0f);
	this->velocity = vec3(0.0f, 0.0f, 0.0f);
	this->setPos(vec3(0.0f, 0.0f, 0.0f));
	this->groundedTimer = 0.0f;
	this->jumpDir = vec2(0.0f, 0.0f);
	this->startingJumpDir = vec2(0.0f, 0.0f);
	this->fallBoxTimer = 0.0f;
	this->lookat(this->resetLookat_dir);
	this->jumpAfterPlatformTimer = 0.0f;

	if (lvlClr) {
		//Add points
		scoreManager.LevelDone();
	}
}

void Player::lookat(vec3 lookat, vec3 offset)
{
	vec3 forward(0, 0, 1);
	vec3 up(0, 1, 0);
	vec3 playerLookAtVec = lookat - getPos();
	float xrot = vec3(playerLookAtVec.x, 0, playerLookAtVec.z).angle(forward);

	playerLookAtVec.Normalize();
	float yrot = acos(playerLookAtVec.Normalize() * up);

	this->cam->setRotation(vec3(-xrot + offset.x, 0, 0));
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
	if (hasShield != true)
	{
		this->movePos(vec3(0, 0.3f, 0));
		this->groundedTimer = 0.51f;
		this->grounded = false;
		this->shoved = true;
		this->shove = shove;
		this->velocity.y = forceY;
		sm->playSound("Hit", getPos());
		shoveDelay = true;	
		ResetGhost();
	}
	else
	{
		hasShield = false;
		getSm()->playSound("Shield2", getPos());
		this->HUD->TurnOffPassive(SHIELD_P);
	}
}
void Player::bouncePlayer(vec2 bounceVec, float forceY)
{

	this->groundedTimer = 0.11f;
	this->grounded = false;	
	this->velocity.y = forceY;
	this->bounced = true;
	this->bounceVec = bounceVec;
	this->jumpDir = vec2(0.f, 0.f);
	this->startingJumpDir = vec2(0.f, 0.f);
	this->startingJumpKey = 'N';
	sm->playSound("Bounce", getPos());
}

void Player::setVelocity(vec3 vel)
{
	this->velocity = vel;
}

vec3 Player::getVelocity() const
{
	return this->velocity;
}

//gets the powerup index from collission handler when one is picked up
void Player::pickedUpPower(Powerup index)
{
	sm->playSound("Pickup", getPos());
	 
	if (index == FEATHER)
	{
		this->HUD->TurnOnPassive(FEATHER_P);
		this->passive_powerup = FEATHER_P;
	}
	else if (index == PEARL && !pearlActive)
	{
		this->HUD->TurnOnPassive(PEARL_P);
		this->passive_powerup = PEARL_P;
	}
	else if (index == POTION)
	{
		this->HUD->TurnOnPassive(POTION_P);
		this->passive_powerup = POTION_P;
	}
	else if (index == SHIELD)
	{
		this->hasShield = true;
		this->HUD->TurnOnPassive(SHIELD_P);
		this->passive_powerup = SHIELD_P;
	}
	else if (index == APPLE)
	{
		HUD->IncreaseHealth();
		this->passive_powerup = APPLE_P;
	}
	else if (index == MONEY)
	{
		AddScore(100);
		this->passive_powerup = MONEY_P;
	}
	else
	{
		this->power_index = index;
		this->HUD->ChangeCurrentPowerUp(this->power_index);
	}
}

Powerup Player::getPlayerPower()
{
	return this->power_index;
}

void Player::setPlayerPower(Powerup index)
{
	this->power_index = index;
	this->HUD->ChangeCurrentPowerUp(index);
}

PowerUpPassiv Player::getPassivePower()
{
	return this->passive_powerup;
}

void Player::setCanDoubleJump()
{
	if (canDoublejump == false)
	{
		canDoublejump = true;
	}
}

bool Player::canDoubleJump()
{
	return this->canDoublejump;
}

void Player::setPlayerPowerPassiv(PowerUpPassiv index)
{
	this->passive_powerup = index;
}

void Player::getShield()
{
	if (this->hasShield == false)
	{
		this->hasShield = true;
	}
}

void Player::setPlayerSpeed(vec3 speed, bool onAndoff)
{
	if (onAndoff == true)
	{
		this->speed.x += speed.x;
		this->speed.z += speed.z;
	}
	else
	{
		this->speed.x = storeSpeed;
		this->speed.z = storeSpeed;
		if (this->speed.x == this->storeSpeed)
		{
			HUD->TurnOffPassive(POTION_P);
		}
	}
}

void Player::useRocket(bool trueOrFalse)
{
	this->usingRocket = trueOrFalse;
	if (trueOrFalse)
	{
		this->grounded = false;
		this->groundedTimer = 0.01f;
		this->rocketTimer = 0.5f;
	}
	else
	{
		this->rocketTimer = 0.0f;
	}
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

void Player::DisableHeart()
{
	this->heardBeat = false;
}

void Player::SetSubmitName(bool val)
{
	submitName = val;
}

void Player::EnableHeart()
{
	this->heardBeat = true;
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

void Player::setBpm(float bpm)
{
	this->bpm = bpm;
}

void Player::setMusicVol(float vol)
{
	musicVol = vol;
}

bool Player::isInvinc() const
{
	return invincible;
}

void Player::set_resetLookat_dir(vec3 lookAt)
{
	this->resetLookat_dir = lookAt;
}

void Player::SetPearl(GameObject*& pearlObj)
{
	this->pearl = pearlObj;
}

void Player::MovePearl(vec3 pos)
{
	if (this->pearlActive == true)
	{
		this->pearl->movePos(pos);
	}
}

void Player::SetPearlPos(vec3 pos)
{
	this->pearl->setPos(pos);
	this->setPearlStatus(true);
	this->HUD->TurnOffPassive(PEARL_P);
}

void Player::PearlHit()
{
	this->setPos(this->pearl->getPos() + vec3(0.0f, 1.5f, 0.0f));
	this->pearl->setPos(vec3(1000.0f, 1000.0f, 1000.0f));
	this->setPearlStatus(false);
}

bool Player::getPearlStatus()
{
	return this->pearlActive;
}

void Player::setPearlStatus(bool trueOrFalse)
{
	this->pearlActive = trueOrFalse;
}

void Player::resetPearl()
{
	SetPearlPos(vec3(1000.0f, 1000.0f, 1000.0f));
	setPearlStatus(false);
}

GameObject*& Player::GetPearl()
{
	if (pearl != nullptr)
	{
		return this->pearl;
	}
	//what to do here
	return pearl;
}

Keyboard* Player::GetKB()
{
	return this->keyboard;
}

void Player::TakeDmg(int dmg)
{
	health-=dmg;
	this->Reset();	
	if(health <= 0 && !this->invincible) {
        alive = false;
		sm->playSound("GameOver", getPos());
	}
	else if(!scream) {
		sm->playSound("Scream", getPos());
	}
	scoreManager.setDamageScore();
	scream = false;
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
	return (int)health;
}

float Player::GetScore()
{
	return scoreManager.GetScore();
}

vec3 Player::GetForwardVec()
{
	return vec3(cam->getForwardVec());
}

bool Player::IsAlive()
{
	return alive;
}

void Player::UpdateFallBox()
{
	fallCube.highPoint = vec3(getPos().x + (fallCubeSize.x/2), getPos().y, getPos().z + (fallCubeSize.z / 2));
	fallCube.lowPoint = vec3(getPos().x - (fallCubeSize.x / 2), getPos().y - fallCubeSize.y, getPos().z - (fallCubeSize.z / 2));
}
