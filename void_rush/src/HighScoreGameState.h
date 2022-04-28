#pragma once
#include "3Dproj/GameState.h"

class HighScoreGameState : public GameState {
public:
	GameState(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam);
	virtual ~GameState();
	//very important that they are done in order
	virtual void handleEvents();//this first
	virtual void renderShadow();//then this
	virtual GameStatesEnum update(float dt);//then this
	virtual void render();		//then this 
};