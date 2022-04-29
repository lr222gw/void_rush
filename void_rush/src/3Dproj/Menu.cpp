#include "Menu.h"

Menu::Menu(Graphics*& gfx, ResourceManager* rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam):
	GameState(gfx, rm, imguimanager, mouse, keyboard, cam),
	soundManager(1)//be able to change this later based on settings
{
	cam->setPosition(vec3(0, 0, 0));
	setUpObject();
	setUpUI();
	
	soundManager.playMusic("assets/audio/MenuMusic.wav", 7.0f);
	soundManager.setMusicLoop(true);
}

Menu::~Menu()
{
	delete GameObjManager;
	delete UI;
	delete skybox;
}

void Menu::handleEvents()
{
}

void Menu::renderShadow()
{
	//nothing here yet
}

GameStatesEnum Menu::update(float dt)
{
	GameStatesEnum theReturn = GameStatesEnum::NO_CHANGE;

	camera->updateCamera();
	camera->addRotation(vec3(0.1 * dt, 0.3 * dt, 0));
	UI->update();
	soundManager.update(camera->getPos(), camera->getForwardVec());

	if (UI->getButton("Quit")->clicked()) {
		theReturn = GameStatesEnum::QUIT;
	}
	else if (UI->getButton("Start")->clicked()) {
		UI->createUIString("Loading...", vec2(-1, -0.5), vec2(0.2, 0.2), "loading");
		theReturn = GameStatesEnum::TO_GAME;
	}
	else if (UI->getButton("HighScores")->clicked()) {
		theReturn = GameStatesEnum::TO_HIGHSCORE;
	}

	return theReturn;
}

void Menu::render()
{
	gfx->setRenderTarget();
	gfx->setTransparant(true);
	
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[0]);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);
	skybox->setPos(camera->getPos());
	skybox->updateVertexShader(gfx);
	skybox->updatePixelShader(gfx);
	skybox->draw(gfx);

	gfx->get_IMctx()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IMctx()->OMSetRenderTargets(1, &gfx->getRenderTarget(), nullptr);
	UI->draw();
	gfx->get_IMctx()->OMSetRenderTargets(1, &gfx->getRenderTarget(), gfx->getDepthStencil());

	gfx->present(0);
}

void Menu::setUpUI()
{
	UI = new UIManager(rm, gfx);
	UI->createUIButton("assets/textures/backbebap.png","END", mouse, vec2(-0.75, 0), vec2(0.5, 0.5), "Quit");
	UI->createUIButton("assets/textures/outline.png","Start", mouse, vec2(0.25, 0), vec2(0.5, 0.5), "Start", vec2(0.02,0.2), vec2(-0.01,0));
	UI->createUIButton("assets/textures/outline.png", "HighScores", mouse, vec2(-0.25, -0.75), vec2(0.5, 0.5), "HighScores", vec2(0.02, 0.2), vec2(-0.01, 0));
}

void Menu::setUpObject()
{
	GameObjManager = new GameObjectManager(gfx, rm);
	std::string skyboxTextures[6] = {
		"assets/textures/Skybox/posx.png",//x+
		"assets/textures/Skybox/negx.png",//x-
		"assets/textures/Skybox/posy.png",//y+
		"assets/textures/Skybox/negy.png",//y-
		"assets/textures/Skybox/posz.png",//z+
		"assets/textures/Skybox/negz.png"//z-
	};
	rm->getSpriteCube(skyboxTextures, gfx);
	skybox = new SkyBox(rm->get_Models("skybox_cube.obj", gfx), gfx, vec3(0,0,0), rm->getSpriteCube(skyboxTextures,gfx));
}
