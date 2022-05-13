#pragma once
#include "3Dproj/GameState.h"

class HighScoreGameState : public GameState {
public:
	HighScoreGameState(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam);
	virtual ~HighScoreGameState();
	//very important that they are done in order
	virtual void handleEvents();//this first
	virtual void renderShadow();//then this
	virtual GameStateRet update(float dt);//then this
	virtual void render();		//then this 
private:
	void readHighScoreFile();
	UIManager* UI;
	SkyBox* skybox;
	static const int numberOfHighScore = 5;
	std::string scores[numberOfHighScore];
	std::string seeds[numberOfHighScore];

};