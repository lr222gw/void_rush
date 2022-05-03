#include "Powerups.h"

Powerups::Powerups(Player* player, ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale, powerups pow)
	: player(player), GameObject(file, gfx, pos, rot, scale), power_index(pow)
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
		std::cout << "Has nothing" << std::endl;
	}
	else if (getPowerUpIndex() == APPLE)
	{
		std::cout << "has apple" << std::endl;
	}
}

powerups Powerups::getPowerUpIndex()
{
	return this->power_index;
}

