#pragma once
#include "3Dproj/GameState.h"
#include "settings.h"

#include <filesystem>

class SettingsScene : public GameState {
public:
	SettingsScene(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam);
	virtual ~SettingsScene();
	//very important that they are done in order
	virtual void handleEvents();//this first
	virtual void renderShadow();//then this
	virtual GameStateRet update(float dt);//then this
	virtual void render();		//then this 
private:
	void readSettings();
	void saveSettings();

	UIManager* UI;
	SkyBox* skybox;
	settings sett;
	std::string VolumeToString();
	std::string ResolutionToString();
	std::string FullScreenToString();
	float savedTime = 0;
	bool exist = false;
	std::string pathToSettingsFile;
};