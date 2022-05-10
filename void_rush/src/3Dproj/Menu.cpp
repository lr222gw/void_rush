#include "Menu.h"

Menu::Menu(Graphics*& gfx, ResourceManager* rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam):
	GameState(gfx, rm, imguimanager, mouse, keyboard, cam),
	soundManager(1)//be able to change this later based on settings
{
	buttonSize = vec2(0.5f, 0.15f);

	cam->setPosition(vec3(0, 0, 0));
	setUpObject();
	setUpUI();
	
	soundManager.playMusic("assets/audio/MenuMusic.wav", 15.0f);
	soundManager.setMusicLoop(true);

	//int nrOfCharsInSeed = std::floorf(std::log10(INT_MAX) + 1);
	inputSeed = false;
	SeedClicked = false;
	currentNumber = 0;
	seed = "__________";
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

GameStateRet Menu::update(float dt)
{
	GameStateRet theReturn;
	theReturn.gameState = GameStatesEnum::NO_CHANGE;
	theReturn.seed = 0;

	camera->updateCamera();
	camera->addRotation(vec3(0.1f * dt, 0.3f * dt, 0));
	UI->update();
	soundManager.update(camera->getPos(), camera->getForwardVec());

	if (UI->getButton("Quit")->clicked()) {
		theReturn.gameState = GameStatesEnum::QUIT;
	}
	else if (UI->getButton("Start")->clicked() || keyboard->isKeyPressed(VK_RETURN)) {
		UI->createUIString("Loading...", vec2(-0.9, -0.75), vec2(0.2, 0.2), "loading");
		theReturn.gameState = GameStatesEnum::TO_GAME;
		theReturn.seed = getSeedInt();
	}
	else if (UI->getButton("HighScores")->clicked()) {
		theReturn.gameState = GameStatesEnum::TO_HIGHSCORE;
	}
	else if (UI->getButton("InputSeed")->clicked()) {
		//Enable seed input
		if (!SeedClicked) {
			if (inputSeed) {
				UI->getStringElement("Seed")->setPosition(vec2(-0.2, 10));
				inputSeed = false;
			}
			else {
				UI->getStringElement("Seed")->setPosition(vec2(-0.2, 0));
				inputSeed = true;
			}
		}
		SeedClicked = true;
	}
	checkHover();
	if (inputSeed) {
		getSeedInput();
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
	UI->draw();

	gfx->present(0);
}

void Menu::setUpUI()
{
	UI = new UIManager(rm, gfx);
	UI->createUIString("Void Rush", vec2(-0.4, 0.7), vec2(0.1, 0.1), "Title");
	UI->createUIButton("assets/textures/buttonBack.png", "Start", mouse, vec2(-0.9, 0.4), buttonSize, "Start", vec2(0.0, 0.0), vec2(0, 0.1));
	UI->createUIButton("assets/textures/buttonBack.png","END", mouse, vec2(-0.9, -0.5), buttonSize, "Quit", vec2(0.0,0.0), vec2(0,0.1));
	UI->createUIButton("assets/textures/buttonBack.png", "HighScores", mouse, vec2(-0.9, 0), buttonSize, "HighScores", vec2(0.0, -0.025), vec2(0.0, 0.1));
	UI->createUIButton("assets/textures/buttonBack.png", "InputSeed", mouse, vec2(0, 0.4), buttonSize, "InputSeed", vec2(0.0, -0.025), vec2(0.0, 0.1));
	UI->createUIString("__________", vec2(-0.2, 10), vec2(0.1, 0.1), "Seed");
	//Used for scaling buttons
	buttonNames.push_back("Start");
	buttonNames.push_back("Quit");
	buttonNames.push_back("HighScores");
	buttonNames.push_back("InputSeed");
	for (int i = 0; i < buttonNames.size(); i++) {
		buttonPos.push_back(UI->getButton(buttonNames[i])->getPosition());
	}
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

void Menu::checkHover()
{
	for (int i = 0; i < buttonNames.size(); i++) {
		if (UI->getButton(buttonNames[i])->hover()) {
			UI->getButton(buttonNames[i])->setSize(buttonSize.x * 1.1f, buttonSize.y * 1.1f);
			UI->getButton(buttonNames[i])->setPosition(buttonPos[i].x - 0.01f, buttonPos[i].y -0.01f);
		}
		else {
			UI->getButton(buttonNames[i])->setSize(buttonSize.x, buttonSize.y);
			UI->getButton(buttonNames[i])->setPosition(buttonPos[i].x + 0.0f, buttonPos[i].y + 0.0f);
		}
	}

	if (!UI->getButton("InputSeed")->clicked()) {
		SeedClicked = false;
	}
}

void Menu::getSeedInput()
{
	if (currentNumber < 10) {
		for (int i = 48; i < 58; i++) {
			if (keyboard->isKeyPressed(i)) {
				seed.at(currentNumber++) = char(i);
				UI->getStringElement("Seed")->setText(seed);
				keyboard->onKeyReleased(i);
			}
		}
	}
	
	if (currentNumber > 0) {
		if (keyboard->isKeyPressed(VK_BACK)) {
			seed.at(--currentNumber) = '_';
			UI->getStringElement("Seed")->setText(seed);
			keyboard->onKeyReleased(VK_BACK);
		}
	}
}

int Menu::getSeedInt()
{
	std::string cleanSeed="";
	for (int i = 0; i < seed.size(); i++) {
		if (seed.at(i) == '_') {
			if (i == 0) {
				cleanSeed = "-1";
			}
			break;
		}
		else {
			cleanSeed += seed.at(i);
		}
	}
	return std::stoi(cleanSeed);
}

