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
	else if (player->getPlayerPower() == ROCKET)
	{
		if (this->keyboard->isKeyPressed('E'))
		{
			std::cout << "Has Rocket" << std::endl;

		}
	}
	else if (player->getPlayerPower() == FREEZE)
	{
		if (this->keyboard->isKeyPressed('E'))
		{
			std::cout << "Has Freeze" << std::endl;

		}
	}
	else if (player->getPlayerPower() == APPLE)
	{
		if (this->keyboard->isKeyPressed('E'))
		{
			std::cout << "picked apple" << std::endl;
			player->getSm()->playSound("GoldApple", player->getPos());
			player->AddHealth();
			player->setPlayerPower(EMPTY);
		}
	}
	
}

Powerup Powerups::getPowerUpIndex()
{
	return this->power_index;
}

//Decides which powerups are used this map.
void Powerups::initiatePowerups()
{
}

