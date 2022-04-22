#pragma once
#include "Graphics.h"
#include "deltaTime.h"
#include "Camera.h"
#include "imguiManager.h"
#include "ResourceManager.h"
//#include "DeferredRendering.h"
#include "TrashCollector.h"
#include "flags.h"
#include "Game.h"

//git
class App {
public:
	App(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	virtual ~App();
	void run();
private:
	MSG msg = {};
	Graphics* gfx;
	ResourceManager* rm;
	const float PI = 3.14159265359f;
private:
	friend class ImguiManager;
	//logic and others
	//DeferredRendering* defRend;
	DeltaTime dt;
	ImguiManager IMGUIManager;
	Mouse* mouse;
	Keyboard* keyboard;
	Camera* camera;
	GameState* gamestate;
	bool quit;
	void handleGamestateChanges(GameStatesEnum handle);
};