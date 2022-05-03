#include "Powerups.h"

Powerups::Powerups(Player* player, Hud* hud, ModelObj* file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale) 
	: player(player), hud(hud), GameObject(file, gfx, pos, rot, scale)
{
}

Powerups::~Powerups()
{
}

void Powerups::UsePowerUp()
{
	//if (PowerUp == EMPTY)
	//{
	//	//Do nothing
	//}
	//else if
}
