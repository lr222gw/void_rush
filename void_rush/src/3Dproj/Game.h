#pragma once
#include "Graphics.h"
#include "deltaTime.h"
#include "Camera.h"
#include "imguiManager.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "BillBoardManager.h"
#include "Light.h"

#include "DeferredRendering.h"
#include "ShadowMap.h"
#include "Graphics.h"
#include "TrashCollector.h"
#include "SoundManager.h"

#include "flags.h"
#include "skybox.h"
#include "UIManager.h"

#include "Collision3D.h"
#include "Ghost.h"
#include "GameObjectManager.h"

#include "puzzle/protoPuzzle.hpp"
#include "generation/Generation_manager.hpp"

#include "interact/interact.hpp"
#include "CollisionHandler.h"

//git
class Game {
public:
	Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	virtual ~Game();
	void run();
	void DrawToBuffer();
	void ForwardDraw();
	void DrawAllShadowObject();//without any otherShaders
	void updateShaders(bool vs = true, bool ps = true);
private:
	float testTime = 0.0f;
	ProtoPuzzle* testPuzzle;
	MSG msg = {};
	Graphics *gfx;
	ResourceManager* rm;
	void Update();
	const float PI = 3.14159265359f;

	void Interact(std::vector<GameObject*>& interactables);
	void HandlePlayer();
private:
	friend class ImguiManager;
	//logic and others
	DeferredRendering *defRend;
	DeltaTime dt;
	ImguiManager IMGUIManager;
	Mouse* mouse;
	Keyboard* keyboard;
	Camera* camera;
	ShadowMap* shadowMap;
	SoundManager soundManager;
	UIManager* UI;
	SkyBox* Space;
	Player* player;
	Ghost* ghost;
	GameObjectManager* GameObjManager;
	Generation_manager* generationManager;
	CollisionHandler collisionHandler;
	void setUpObject();
	void setUpLights();
	void setUpParticles();

	//game objects
	Light **light;
	std::vector<GameObject*> LightVisualizers;
	std::vector<GameObject*> obj;
	std::vector<BillBoardGroup*> billboardGroups;

	float maxDepth;

	//var
	int nrOfLight;//must still exist

	//debug var
	int lightNr;
};