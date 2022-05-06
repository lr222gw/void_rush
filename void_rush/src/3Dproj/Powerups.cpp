#include "Powerups.h"

Powerups::Powerups(ModelObj* file, Graphics*& gfx, Player* player, Keyboard* keyboard, vec3 pos, vec3 rot, vec3 scale, Powerup pow)
	: GameObject(file, gfx, pos, rot, scale), power_index(pow)
{
	this->player = player;
	this->keyboard = keyboard;
}

Powerups::~Powerups()
{
}

void Powerups::update(float dt)
{
	UsePowerUp();
	
}

void Powerups::UsePowerUp()
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
	else if (player->getPlayerPower() == FREEZE)
	{
		if (this->keyboard->isKeyPressed('E'))
		{
			////ADD HERE WHAT FREEZE DOES WHEN ACTIVATED////
			player->getSm()->playSound("Freeze", player->getPos());
			std::cout << "Has Freeze" << std::endl;
		}
	}
	else if (player->getPlayerPower() == PEARL)
	{
		////ADD HERE WHAT PEARL DOES WHEN ACTIVATED////
		//player->getSm()->playSound("Pearl", player->getPos());
	}
	else if (player->getPlayerPower() == EMP)
	{
		////ADD HERE WHAT PEARL DOES WHEN ACTIVATED////
		//player->getSm()->playSound("EMP", player->getPos());
	}
	else if (player->getPlayerPower() == PAD)
	{
		////ADD HERE WHAT TRAMPOLINE DOES WHEN ACTIVATED////
		//player->getSm()->playSound("Pad", player->getPos());
	}
	else if (player->getPlayerPower() == FEATHER)
	{
		////ADD HERE WHAT FEATHER DOES WHEN ACTIVATED////
		//player->getSm()->playSound("Feather", player->getPos());
	}
	else if (player->getPlayerPower() == POTION)
	{
		////ADD HERE WHAT POTION DOES WHEN ACTIVATED////
		//player->getSm()->playSound("Potion", player->getPos());
	}
	else if (player->getPlayerPower() == SHIELD)
	{
		////ADD HERE WHAT SHEILD DOES WHEN ACTIVATED////
		//player->getSm()->playSound("Shield", player->getPos());
	}
	else if (player->getPlayerPower() == MONEY)
	{
		////ADD HERE WHAT MONEY DOES WHEN ACTIVATED////
		//player->getSm()->playSound("Money", player->getPos());
	}
}

Powerup Powerups::getPowerUpIndex()
{
	return this->power_index;
}