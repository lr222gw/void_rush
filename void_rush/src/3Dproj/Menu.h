#pragma once
#include "SoundManager.h"
#include "GameState.h"

class Menu : public GameState {
public:
	Menu(Graphics*& gfx, ResourceManager* rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam);
	virtual ~Menu();
	virtual void handleEvents();//this first
	virtual void renderShadow();//then this
	virtual GameStateRet update(float dt);//then this
	virtual void render();		//then this 
private:
	void setUpUI();
	void setUpObject();
	void checkHover();
	void getSeedInput();
	int getSeedInt();

	GameObjectManager* GameObjManager;
	UIManager* UI;
	SkyBox* skybox;
	SoundManager soundManager;

	vec2 buttonSize;
	std::vector<std::string>buttonNames;
	std::vector<vec2>buttonPos;

	bool inputSeed;
	bool SeedClicked;
	std::string seed;
	int currentNumber;
	float heightObject;
};