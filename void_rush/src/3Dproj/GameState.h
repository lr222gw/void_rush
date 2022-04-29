#pragma once
#include "GameObjectManager.h"
#include "CollisionHandler.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Light.h"
#include "skybox.h"
#include "UIManager.h"
#include "Ghost.h"
#include "imguiManager.h"
#include "flags.h"

enum class GameStatesEnum { NO_CHANGE, QUIT, TO_MENU, TO_GAME, TO_HIGHSCORE };

class GameState {
public:
	GameState(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam);
	virtual ~GameState();
	//very important that they are done in order
	virtual void handleEvents();//this first
	virtual void renderShadow();//then this
	virtual GameStatesEnum update(float dt);//then this
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