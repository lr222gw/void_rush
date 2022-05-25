#pragma once
#include "GameState.h"
#include "ShadowMap.h"
#include "SoundManager.h"
#include "puzzle/protoPuzzle.hpp"
#include "generation/Generation_manager.hpp"
#include "interact/interact.hpp"
#include "BillBoardManager.h"
#include "hud/Hud.h"
#include "3DLettersHandler.h"
#include "powerups/powerUpManager.hpp"
#include "enemies/EnemyManager.hpp"

#include <cinttypes>


class Game : public GameState {
public:
	Game(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam, GameStateRet extra);
	virtual ~Game();
	//very important that they are done in order
	virtual void handleEvents();//this first
	virtual void renderShadow();//then this
	virtual GameStateRet update(float dt);//then this
	virtual void render();		//then this 
private:
	Letters3D* text;

	void UpdatePauseMenuButtons();
	void updateShaders(bool vs = true, bool ps = true);
	bool pauseMenu;
	float testTime = 0.0f;
	float deltaT = 0.0f;
	int start_seed;
	ProtoPuzzle* puzzleManager;
	ShadowMap* shadowMap;
	SkyBox* skybox;
	SoundManager soundManager;
	UIManager* UI;
	UIManager* pauseUI;
	Player* player;	
		 

	Ghost* ghost;	
	GameObjectManager* GameObjManager;
	Generation_manager* generationManager;
	CollisionHandler collisionHandler;
	PowerupManager* powerupManager;
	EnemyManager* enemyManager;
	Hud* HUD;
	int testInt = 0;
	float distanceFromStartPosToPuzzle = 0.0f;
	Letters3DHandler* letter3DHandler;

	/*draw to buffer*/
	void ForwardDraw();
	void DrawToBuffer();

	/*set up things*/
	void setUpObject(GameStateRet extra);
	void setUpLights();
	void setUpParticles();
	void setUpUI();
	void setUpSound();
	void Interact(std::vector<GameObject*>& interactables);
	void SetName();

	//game objects
	Light** light;
	std::vector<BillBoardGroup*> billboardGroups;

	//Menu buttons vars
	vec2 buttonSize, continuePos, menuPos;


	//var
	int nrOfLight;//must still exist

	//debug var
	int lightNr;
	//Debug
	friend class ImguiManager;
};