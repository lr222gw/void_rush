#include "Powerups.h"

Powerups::Powerups(ModelObj* file, Graphics*& gfx, Player* player, Ghost* ghost, Mouse* mouse, Keyboard* keyboard, vec3 pos, vec3 rot, vec3 scale, Powerup pow)
	: GameObject(file, gfx, pos, rot, scale), power_index(pow)
{
	this->player = player;
	this->keyboard = keyboard;
	this->mouse = mouse;
	this->ghostFrozenTimer = 0.0f;
	this->ghost = ghost;
	this->featherActive = false;
	this->potionActive = false;
	this->potionTimer = 0.0f;
	this->rocketActive = false;
	this->rocketTimer = 0.0f;
}

Powerups::~Powerups()
{
}

void Powerups::update(float dt)
{
	UsePowerUp(dt);
}

void Powerups::UsePowerUp(float dt)
{
	if (getPowerUpIndex() == EMPTY)
	{
		//Do nothing
	}
	if (player->getPassivePower() == APPLE_P)
	{
		std::cout << "picked apple" << std::endl;
		player->getSm()->playSound("GoldApple", player->getPos());
		player->AddHealth();
		player->setPlayerPowerPassiv(EMPTY_P);
	}
	if (player->getPlayerPower() == ROCKET || this->rocketActive == true)
	{
		if (this->keyboard->isKeyPressed('E'))
		{
			if (rocketActive == false)
			{
				this->rocketActive = true;
				player->setPlayerPower(EMPTY);
				player->useRocket(true);
				player->getSm()->playSound("Rocket", player->getPos());
			}
		}
		if (this->rocketActive == true)
		{
			rocketTimer += dt;
		}
		if (rocketTimer >= 3.0f)
		{
			player->getSm()->stopSound("Rocket");
			rocketTimer = 0.0f;
			player->useRocket(false);
			rocketActive = false;
		}
	}
	if (player->getPlayerPower() == FREEZE || ghost->isFrozen())
	{
		if (this->keyboard->isKeyPressed('E') && !ghost->isFrozen())
		{
			////ADD HERE WHAT FREEZE DOES WHEN ACTIVATED////
			player->getSm()->playSound("Freeze", player->getPos());
			ghostFrozenTimer = 0.1f;
			ghost->freezeGhost();
			player->setPlayerPower(EMPTY);
			std::cout << "Frozen: " << std::endl;
		}
		if (ghostFrozenTimer >= 10.0f)
		{
			ghost->freezeGhost();
			ghostFrozenTimer = 0.0f;
			std::cout << "unfrozen" << std::endl;
		}
		if (ghostFrozenTimer != 0.0f)
		{
			ghostFrozenTimer += dt;
		}
	}
	if (player->getPassivePower() == PEARL_P || pearlActive == true)
	{
		if (player->getPassivePower() == PEARL_P)
		{
			player->setPlayerPowerPassiv(EMPTY_P);
			this->pearlActive = true;
		}

		if (mouse->isRightDown() && pearlTime == 0.0f)
		{
			pearlTime += dt;
			player->SetPearlPos(player->getPos());
			this->pearlVec = player->GetForwardVec();
			player->getSm()->playSound("Pearl", player->getPos());
		}
		else if (pearlTime > 0.0f)
		{
			if (!player->getPearlStatus())
			{
				this->pearlActive = false;
			}
			else
			{
				player->MovePearl(pearlVec / 3.0f);
				pearlTime += dt;
			}

			if (pearlTime >= 3.0f)
			{
				this->pearlActive = false;
				this->pearlTime = 0;
				this->pearlVec = vec3(0.0f, 0.0f, 0.0f);
				player->resetPearl();
				player->setPearlStatus(false);
			}
		}

		if (!player->getPearlStatus() && pearlTime > 0.0f)
		{
			this->pearlTime = 0.0f;
			this->pearlVec = vec3(0.0f, 0.0f, 0.0f);
		}

	}
	if (player->getPassivePower() == FEATHER_P || featherActive == true)
	{
		if (player->getPassivePower() == FEATHER_P)
		{
			player->setPlayerPowerPassiv(EMPTY_P);
		}
		if (this->featherActive == false)
		{
			player->setPlayerPowerPassiv(EMPTY_P);
			this->featherActive = true;
		}
		if (keyboard->onceisKeyReleased(VK_SPACE) && !player->isGrounded())
		{
			player->setCanDoubleJump();
			this->featherbooltest = true;
		}

		if (keyboard->isKeyPressed(VK_SPACE) && player->canDoubleJump() && featherbooltest)
		{
			featherActive = false;
		}
		else if(player->isGrounded())
		{
			featherbooltest = false;
		}
	}
	if (player->getPassivePower() == POTION_P || potionActive == true)
	{
		if (player->getPassivePower() == POTION_P)
		{
			player->setPlayerPowerPassiv(EMPTY_P);
		}
		player->getSm()->playSound("Potion", player->getPos());
		if (potionActive == false)
		{
			potionActive = true;
			//player->setPlayerPowerPassiv(EMPTY_P);
			player->setPlayerSpeed(vec3(2.0f, 0.0f, 2.0f), true);
		}
		if (potionTimer >= 10.0f)
		{
			potionTimer = 0.0f;
			player->setPlayerSpeed(vec3(-2.f, 0.0f, -2.f), false);
			potionActive = false;

		}
		potionTimer += dt;
	}
	if (player->getPassivePower() == SHIELD_P)
	{
		player->getSm()->playSound("Shield", player->getPos());
		player->setPlayerPowerPassiv(EMPTY_P);
		
	}
	if (player->getPassivePower() == MONEY_P)
	{
		player->getSm()->playSound("Money", player->getPos());
		player->setPlayerPowerPassiv(EMPTY_P);
	}
}

Powerup Powerups::getPowerUpIndex()
{
	return this->power_index;
}