#pragma once
#include "3Dproj/GameObjectManager.h"
#include "3Dproj/CollisionHandler.h"
#include "3Dproj/Graphics.h"
#include "3Dproj/vec.h"
#include "3Dproj/Player.h"
#include "3Dproj/Ghost.h"
#include "3Dproj/Mouse.h"
#include "3Dproj/Keyboard.h"
#include "powerups/PowerUpDefs.hpp"
#include "3Dproj/Powerups.h"

class PowerupManager {

private:
	GameObjectManager* GameObjManager;
	CollisionHandler* collisionHandler;
	ResourceManager* rm;
	Graphics* gfx;
	Player* player;
	Ghost* ghost;
	Keyboard* keyboard;
	Mouse* mouse;
	std::vector<Powerups*> powers;
	struct nrOfPowerups {
		int GoldenApples= 0;
		int Potions	= 0;
		int Feathers= 0;
		int Shields	= 0;
		int Money	= 0;
		int Snowflakes= 0;
		int Pearls	= 0;
		int EMPs	= 0;
		int Kills	= 0;
		int Pads	= 0;
		int Rockets	= 0;
		int Cards	= 0;
	}nrOf,nrOfActive;

	

	void create_GoldenApple();
	void create_Feather();
	void create_Potion();
	void create_Shield();
	void create_Money();
	void create_Snowflake();
	void create_Pearl();
	void create_EMP();
	void create_Pad();
	void create_Kill();
	void create_Rocket();
	void create_Card();

public: 
	PowerupManager(GameObjectManager* GameObjManager, Graphics* gfx, ResourceManager* rm, CollisionHandler* collisionHandler, Mouse* mouse, Keyboard* keyboard);
	void init(Player* player, Ghost* ghost);
	//Decides which powerups are used this map.
	void reset();

	GameObject*& get_GoldenApple();
	GameObject*& get_Feather();
	GameObject*& get_Potion();
	GameObject*& get_Shield();
	GameObject*& get_Money();
	GameObject*& get_Snowflake();
	GameObject*& get_Pearl();
	GameObject*& get_EMP();
	GameObject*& get_Pad();
	GameObject*& get_Kill();
	GameObject*& get_Rocket();
	GameObject*& get_Card();
	


	void setUpPowerups(int chosenDiff, vec3 pos);


};