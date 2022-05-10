#include "Powerups.h"

Powerups::Powerups(ModelObj* file, Graphics*& gfx, Player* player, Ghost* ghost, Keyboard* keyboard, vec3 pos, vec3 rot, vec3 scale, Powerup pow)
	: GameObject(file, gfx, pos, rot, scale), power_index(pow)
{
	this->player = player;
	this->keyboard = keyboard;
	this->ghostFrozenTimer = 0.0f;
	this->ghost = ghost;
	this->featherActive = false;
	this->featherTimer = 0.0f;
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
	else if (player->getPlayerPower() == APPLE)
	{
		std::cout << "picked apple" << std::endl;
		player->getSm()->playSound("GoldApple", player->getPos());
		player->AddHealth();
		player->setPlayerPower(EMPTY);
	}
	else if (player->getPlayerPower() == KILL)
	{
		////ADD HERE WHAT KILL DOES WHEN ACTIVATED////
	}
	else if (player->getPlayerPower() == ROCKET)
	{
		if (this->keyboard->isKeyPressed('E'))
		{
			////ADD HERE WHAT ROCKET DOES WHEN ACTIVATED////
			player->getSm()->playSound("Rocket", player->getPos());
			std::cout << "Has Rocket" << std::endl;
		}
	}
	else if (player->getPlayerPower() == CARD)
	{
		////ADD HERE WHAT CARD DOES WHEN ACTIVATED////
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
	else if (player->getPlayerPower() == PEARL)
	{
		////ADD HERE WHAT PEARL DOES WHEN ACTIVATED////
	}
	else if (player->getPlayerPower() == EMP)
	{
		////ADD HERE WHAT PEARL DOES WHEN ACTIVATED////
	}
	else if (player->getPlayerPower() == PAD)
	{
		////ADD HERE WHAT TRAMPOLINE DOES WHEN ACTIVATED////
	}
	if (player->getPlayerPower() == FEATHER || featherActive == true)
	{
		////ADD HERE WHAT FEATHER DOES WHEN ACTIVATED////
		if (this->featherActive == false)
		{
			player->setPlayerPower(EMPTY);
		}
		this->featherActive = true;
		if (keyboard->onceisKeyReleased(VK_SPACE) && !player->isGrounded())
		{
			player->setCanDoubleJump();
			
			this->featherActive = false;

		}
		
	}
	else if (player->getPlayerPower() == POTION)
	{
		////ADD HERE WHAT POTION DOES WHEN ACTIVATED////
	}
	else if (player->getPlayerPower() == SHIELD)
	{
		////ADD HERE WHAT SHEILD DOES WHEN ACTIVATED////
	}
	else if (player->getPlayerPower() == MONEY)
	{
		////ADD HERE WHAT MONEY DOES WHEN ACTIVATED////
	}
}

Powerup Powerups::getPowerUpIndex()
{
	return this->power_index;
}