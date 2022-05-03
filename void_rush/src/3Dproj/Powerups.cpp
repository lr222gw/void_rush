#include "Powerups.h"

Powerups::Powerups(Player* player, ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale, powerups pow)
	: player(player), GameObject(file, gfx, pos, rot, scale)
{
}

Powerups::~Powerups()
{
}

void Powerups::UsePowerUp()
{
	
	if (getPowerUpIndex() == EMPTY)
	{
		//Do nothing
	}
	else if (getPowerUpIndex() == APPLE)
	{
		std::cout << "Har Äpple" << std::endl;
	}
}

powerups Powerups::getPowerUpIndex()
{
	return this->power_index;
}

