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

enum class GameStatesEnum { NO_CHANGE, QUIT, TO_MENU, TO_GAME };

class GameState {
public:
	GameState(Graphics*& gfx, ResourceManager* rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard);
	//very important that they are done in order
	void handleEvents();//this first
	void renderShadow();//then this
	GameStatesEnum update();//then this
	void render();		//then this 
	
private:
	Graphics* gfx;
	ResourceManager* rm;
	ImguiManager* IMGUI;
	Mouse* mouse;
	Keyboard* keyboard;
};