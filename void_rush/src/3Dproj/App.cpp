#include "App.h"

App::App(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow, mouse);
	mouse = gfx->getWindosClass().getMouse();
	keyboard = gfx->getWindosClass().getKeyboard();
	quit = false;

	//defRend = new DeferredRendering(gfx);
	rm = new ResourceManager(gfx);

	camera = new Camera(gfx, mouse, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));
	camera->setData();
	gfx->takeIM(&this->IMGUIManager);
	
	//Create a buffer for the light const buffer(hlsli)
	CreateConstBuffer(gfx, gfx->getConstBuffers(0), sizeof(*gfx->getLightconstbufferforCS()), gfx->getLightconstbufferforCS());
	CreateConstBuffer(gfx, gfx->getConstBuffers(1), sizeof(*gfx->getCamPosconstbuffer()), gfx->getCamPosconstbuffer());

	gamestate = new Game(gfx, rm, &IMGUIManager, mouse, keyboard, camera);
	
}

App::~App()
{
	//part of game
	TC::GetInst().empty();
	delete gfx;
	delete rm;

	//logic and other
	//delete defRend;
	delete camera;
}


void App::run()
{
	while (msg.message != WM_QUIT && gfx->getWindosClass().ProcessMessages() && !quit)
	{
		if (dt.dt() > 0.2f) {
			dt.setDeltaTime(0.2f);
		}
		//handle events
		gamestate->handleEvents();

		//Shadow
		gfx->clearScreen();
		gfx->setTransparant(false);
		//for shadow
		//måste uppdatera detta så inte hela object uppdateras när bara skugga ska
		vec3 camLP = camera->getPos();
		vec3 camLR = camera->getRot();
		gamestate->renderShadow();
		//set cam position so its the real cam
		camera->setPosition(camLP);
		camera->setRotation(camLR);
		gfx->setProjection(0);//last can be dir light
		gfx->RsetViewPort();
		

		gamestate->update(dt.dt());

		gamestate->render();

		dt.restartClock();
	}
	printf("quit");
}

void App::handleGamestateChanges(GameStatesEnum handle)
{
	switch (handle) {
	case GameStatesEnum::QUIT:
		quit = true;
		break;
	case GameStatesEnum::TO_GAME:
		//delete current gamestate
		//set gamestate to Game
		break;
	case GameStatesEnum::TO_MENU:
		//delete current gamestate
		//set gamestate to Menu
		break;
	}
}

