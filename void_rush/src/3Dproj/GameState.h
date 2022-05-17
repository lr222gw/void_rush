#pragma once
#include "GameObjectManager.h"
#include "CollisionHandler.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Light.h"
#include "skybox.h"
#include "UIManager.h"
#include "enemies/Ghost.h"
#include "powerups/Powerups.h"
#include "imguiManager.h"
#include "flags.h"

enum class GameStatesEnum { NO_CHANGE, QUIT, TO_MENU, TO_GAME, TO_HIGHSCORE, TO_SETTINGS };

//Return struct
struct GameStateRet {
	GameStatesEnum gameState;
	int seed;
};

class GameState {
public:
	GameState(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam);
	virtual ~GameState();
	//very important that they are done in order
	virtual void handleEvents();//this first
	virtual void renderShadow();//then this
	virtual GameStateRet update(float dt);//then this
	virtual void render();		//then this 
//protected:
public:
	Camera* camera;
	Graphics* gfx;
	ResourceManager* rm;
	ImguiManager* IMGUI;
	Mouse* mouse;
	Keyboard* keyboard;
};