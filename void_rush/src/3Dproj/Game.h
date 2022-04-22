#pragma once
#include "GameState.h"
#include "ShadowMap.h"
#include "SoundManager.h"
#include "puzzle/protoPuzzle.hpp"
#include "generation/Generation_manager.hpp"
#include "interact/interact.hpp"
#include "BillBoardManager.h"
#include "flags.h"
#include "TrashCollector.h"

class Game : public GameState {
public:
	Game(Graphics*& gfx, ResourceManager* rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam);
	virtual ~Game();
	//very important that they are done in order
	virtual void handleEvents();//this first
	virtual void renderShadow();//then this
	virtual GameStatesEnum update(float dt);//then this
	virtual void render();		//then this 
private:
	void updateShaders(bool vs = true, bool ps = true);
	float testTime = 0.0f;
	ProtoPuzzle* testPuzzle;
	ShadowMap* shadowMap;
	SkyBox* skybox;
	SoundManager soundManager;
	UIManager* UI;
	Player* player;
	Ghost* ghost;
	GameObjectManager* GameObjManager;
	Generation_manager* generationManager;
	CollisionHandler collisionHandler;

	/*draw to buffer*/
	void ForwardDraw();
	void DrawToBuffer();

	/*set up things*/
	void setUpObject();
	void setUpLights();
	void setUpParticles();
	void setUpUI();
	void setUpSound();
	void Interact(std::vector<GameObject*>& interactables);
	void HandlePlayer();

	//game objects
	Light** light;
	std::vector<GameObject*> LightVisualizers;
	std::vector<BillBoardGroup*> billboardGroups;

	float maxDepth;

	//var
	int nrOfLight;//must still exist

	//debug var
	int lightNr;
};