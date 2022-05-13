#include "Menu.h"

Menu::Menu(Graphics*& gfx, ResourceManager* rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam):
	GameState(gfx, rm, imguimanager, mouse, keyboard, cam),
	soundManager(1)//be able to change this later based on settings
{
	buttonSize = vec2(0.5f, 0.15f);
	camera->setPosition(vec3(0, 0, 0));
	camera->setRotation(vec3(0, 0, 0));
	setUpObject();
	setUpLights();
	this->shadowMap = new ShadowMap((SpotLight**)lights, nrOfLights, gfx, (UINT)gfx->getClientWH().x, (UINT)gfx->getClientWH().y);
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
	for (int i = 0; i < nrOfLights; i++) {
		delete lights[i];
	}
	delete lights;
	delete shadowMap;
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
	theReturn.seed = -1;

	camera->updateCamera();
	gfx->Update(dt);
	//camera->addRotation(vec3(0.1f * dt, 0.3f * dt, 0));
	skybox->addRot(vec3(0.0f, -0.08f * dt, 0.09f * dt));
	UI->update();
	soundManager.update(camera->getPos(), camera->getForwardVec());

	if (UI->getButton("Quit")->clicked()) {
		theReturn.gameState = GameStatesEnum::QUIT;
	}
	else if (UI->getButton("Start")->clicked() || keyboard->isKeyPressed(VK_RETURN)) {
		UI->createUIString("Loading...", vec2(-0.9f, -0.75f), vec2(0.2f, 0.2f), "loading");
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
				UI->getStringElement("Seed")->setPosition(vec2(-0.2f, 10.0f));
				inputSeed = false;
			}
			else {
				UI->getStringElement("Seed")->setPosition(vec2(-0.2f, 0.0f));
				inputSeed = true;
			}
		}
		SeedClicked = true;
	}
	heightObject += dt;
	if (heightObject >= 2 * 3.14) {
		heightObject = 0;
	}
	GameObjManager->getGameObject("Ghost")->setPos(vec3(3, (sin(heightObject) * 0.5f) - 3.0f, 10.0f));

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
	gfx->get_IMctx()->PSSetShaderResources(1, 1, &shadowMap->GetshadowResV());
	
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[0]);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);
	skybox->setPos(camera->getPos());
	skybox->updateVertexShader(gfx);
	skybox->updatePixelShader(gfx);
	skybox->draw(gfx);

	gfx->get_IMctx()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[0], nullptr, 0);
	GameObjManager->updatePixel();
	GameObjManager->updateVertex();
	GameObjManager->draw();
	UI->draw();

	gfx->present(0);
}

void Menu::setUpUI()
{
	UI = new UIManager(rm, gfx);
	UI->createUIString("Void Rush", vec2(-0.4f, 0.7f), vec2(0.1f, 0.1f), "Title");
	UI->createUIButton("assets/textures/buttonBack.png", "Start", mouse, vec2(-0.9f, 0.4f), buttonSize, "Start", vec2(0.0f, 0.0f), vec2(0.0f, 0.1f));
	UI->createUIButton("assets/textures/buttonBack.png","END", mouse, vec2(-0.9f, -0.5f), buttonSize, "Quit", vec2(0.0f,0.0f), vec2(0.0f,0.1f));
	UI->createUIButton("assets/textures/buttonBack.png", "HighScores", mouse, vec2(-0.9f, 0.0f), buttonSize, "HighScores", vec2(0.0f, -0.025f), vec2(0.0f, 0.1f));
	UI->createUIButton("assets/textures/buttonBack.png", "InputSeed", mouse, vec2(0.0f, 0.4f), buttonSize, "InputSeed", vec2(0.0f, -0.025f), vec2(0.0f, 0.1f));
	UI->createUIString("__________", vec2(-0.2f, 10.0f), vec2(0.1f, 0.1f), "Seed");
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
	GameObjManager->CreateGameObject("ghost.obj", "Ghost", vec3(0, 0, 20), vec3(0,0.7f,0));
	rm->getSpriteCube(skyboxTextures, gfx);
	skybox = new SkyBox(rm->get_Models("skybox_cube.obj", gfx), gfx, vec3(0,0,0), rm->getSpriteCube(skyboxTextures,gfx));
}

void Menu::setUpLights()
{
	//current max number is set in graphics.cpp and transforms.hlsli
	nrOfLights = 1;
	lights = new Light * [nrOfLights];

	lights[0] = new PointLight(vec3(2.f, 0.f, 8.8f), 0.1, vec3(1, 0.3, 0.3));

	//say to graphics/shaders how many lights we have
	gfx->getLightconstbufferforCS()->nrOfLights.element = nrOfLights;
	gfx->takeLight(lights, nrOfLights);
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
	if (!inputSeed) {
		return -1;
	}
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

