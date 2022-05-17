#pragma once

#include "3Dproj/GameObjectManager.h"
#include "3Dproj/CollisionHandler.h"
#include "3Dproj/Graphics.h"
#include "3Dproj/vec.h"
#include "3Dproj/Player.h"
#include "enemies/Ghost.h"
#include "3Dproj/Mouse.h"
#include "3Dproj/Keyboard.h"
#include "powerups/PowerUpDefs.hpp"
#include "powerups/Powerups.h"
#include "3Dproj/deltaTime.h"
#include "3Dproj/SoundManager.h"
#include "EnemySettings.hpp"

class EnemyManager{
private: 
	friend class ImguiManager;
	GameObjectManager* GameObjManager;
	CollisionHandler* collisionHandler;
	SoundManager* soundManager;
	ResourceManager* rm;
	Graphics* gfx;
	Player* player;
	Ghost* ghost;
	Keyboard* keyboard;
	Mouse* mouse;
	static vec3 FarFarFarAway;
	struct nrOfEnemies {
		int TURRET = 0;
		int SPIKES = 0;
		int SNARE = 0;
		
	}nrOf, nrOfActive;

	void createTurret();
	void createSpikes();
	void createSnare();

	Enemy* get_Turret();
	Enemy* get_Spikes();
	Enemy* get_Snare();

	std::vector<Enemy*> enemies;
	
public:
	EnemyManager(GameObjectManager* GameObjManager, Graphics* gfx, 
		ResourceManager* rm, CollisionHandler* collisionHandler, SoundManager* soundManager,
		Mouse* mouse, Keyboard* keyboard);

	void init(Player* player, Ghost* ghost);
	void reset();

	//void spawnEnemy(std::vector<vec3> pos);
	void spawnEnemy(Enemy_positions::PositionGroup poses);
	void spawnEnemies(Enemy_positions* enemyPoses);

};