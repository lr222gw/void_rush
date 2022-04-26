#pragma once
#include "SoundManager.h"
#include "GameState.h"

class Menu : public GameState {
public:
	Menu(Graphics*& gfx, ResourceManager* rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam);
	virtual ~Menu();
	virtual void handleEvents();//this first
	virtual void renderShadow();//then this
	virtual GameStatesEnum update(float dt);//then this
	virtual void render();		//then this 
private:
	void setUpUI();
	void setUpObject();

	GameObjectManager* GameObjManager;
	UIManager* UI;
	SkyBox* skybox;
	SoundManager soundManager;
};