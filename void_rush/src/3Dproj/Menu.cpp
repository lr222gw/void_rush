#include "Menu.h"

Menu::Menu(Graphics*& gfx, ResourceManager* rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam):
	GameState(gfx, rm, imguimanager, mouse, keyboard, cam),
	soundManager(1)//be able to change this later based on settings
{
	cam->setPosition(vec3(0, 0, 0));
	setUpObject();
	setUpUI();
	//this->IMGUI->set_owner(this);
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

	UI->update();

	if (UI->getButton("Quit")->clicked()) {
		theReturn = GameStatesEnum::QUIT;
	}
	else if (UI->getButton("Start")->clicked()) {
		UI->createUIString("Loading...", vec2(-1, -0.5), vec2(0.2, 0.2), "loading");
		theReturn = GameStatesEnum::TO_GAME;
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
	skybox->draw(gfx);

	gfx->get_IMctx()->VSSetShader(gfx->getVS()[0], nullptr, 0);

	UI->draw();

	gfx->present(0);
}

void Menu::setUpUI()
{
	UI = new UIManager(rm, gfx);
	UI->createUIButton("assets/textures/backbebap.png", mouse, vec2(-0.75, 0), vec2(0.5, 0.5), "Quit");
	UI->createUIButton("assets/textures/outline.png", mouse, vec2(0.25, 0), vec2(0.5, 0.5), "Start");
}

void Menu::setUpObject()
{
	GameObjManager = new GameObjectManager(gfx, rm);
	std::string skyboxTextures[6] = {
	"assets/textures/Skybox/north.png",//x+
	"assets/textures/Skybox/south.png",//x-
	"assets/textures/Skybox/above.png",//y+
	"assets/textures/Skybox/below.png",//y-
	"assets/textures/Skybox/east.png",//z-
	"assets/textures/Skybox/west.png",//z+
	};
	skybox = new SkyBox(rm->get_Models("skybox_cube.obj", gfx), gfx, vec3(0,0,0), skyboxTextures);
}
