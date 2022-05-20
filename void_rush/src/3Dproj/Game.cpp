#include "Game.h"


Game::Game(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam, int seed):
	GameState(gfx,rm,imguimanager,mouse,keyboard,cam),
	soundManager()//be able to change this later based on settings
{
	/*sets in setup___*/
	GameObjManager = new GameObjectManager(gfx, rm);
	UI = nullptr;
	ghost = nullptr;
	nrOfLight = 0; 
	player = nullptr;
	skybox = nullptr;
	
	HUD = new Hud(gfx, rm);
	lightNr = 0;
	puzzleManager = new ProtoPuzzle(gfx, rm, collisionHandler, &soundManager); //TODO: REMOVE COMMENT

	powerupManager	= new PowerupManager(GameObjManager, gfx, rm, &this->collisionHandler, mouse, keyboard);
	enemyManager	= new EnemyManager(GameObjManager, gfx, rm, &this->collisionHandler, &soundManager, mouse, keyboard);
	
	generationManager = new Generation_manager(gfx, rm, collisionHandler, seed);
	generationManager->set_PuzzleManager(puzzleManager); //TODO: REMOVE COMMENT
	generationManager->set_GameObjManager(GameObjManager);
	generationManager->set_PowerupManager(powerupManager);
	generationManager->set_EnemyManager(enemyManager);
	
	camera->setRotation(vec3(0, 0, 0));
	pauseMenu = false;
	
	letter3DHandler = new Letters3DHandler(rm, gfx);

	
	
	/*set ups*/
	this->setUpObject();
	this->setUpLights();
	//this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx, (UINT)gfx->getClientWH().x, (UINT)gfx->getClientWH().y);
	this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx, (UINT)1920, (UINT)1920);
	this->setUpParticles();
	this->setUpSound();
	this->setUpUI();
	this->IMGUI->set_owner(this);

}

Game::~Game()
{
	if (shadowMap != nullptr) {
		delete shadowMap;
	}

	//objects
	for (int i = 0; i < nrOfLight; i++) {
		delete light[i];
	}
	delete[] light;
	for (int i = 0; i < billboardGroups.size(); i++) {
		delete billboardGroups[i];
	}
	
	delete skybox;
	delete puzzleManager; //TODO: REMOVE COMMENT
	delete generationManager;
	delete HUD;
	delete UI;
	delete pauseUI;
	delete GameObjManager;
	delete letter3DHandler;
	delete powerupManager;
	delete enemyManager;
}

void Game::handleEvents()
{
	/*Read Mouse*/
	while (!mouse->EventBufferEmpty() && mouse->getMouseActive()) {
		mouseEvent e = mouse->ReadEvent();
		if (e.getType() == mouseEvent::EventType::RAW_MOVE && !pauseMenu) {
			player->rotateWithMouse(e.getPosX(), e.getPosY());
		}
	}
	if (keyboard->onceisKeyReleased(VK_ESCAPE) && player->IsAlive()) {
		//set pause
		pauseMenu = !pauseMenu;
		if (pauseMenu) {
			gfx->getWindosClass().ShowCoursor();
		}
		else {
			gfx->getWindosClass().HideCoursor();
		}
	}
}

void Game::renderShadow()
{
	shadowMap->setUpdateShadow();
	for (int i = 0; i < nrOfLight; i++) {
		//set cam position to lightposition
		camera->setPosition(light[i]->getPos());
		camera->setRotation(light[i]->getRotation());
		shadowMap->inUpdateShadow(i);
		updateShaders(true, false);
	
		gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);
		gfx->get_IMctx()->PSSetShader(nullptr, nullptr, 0);
		GameObjManager->drawShadow();
	}
}

GameStateRet Game::update(float dt)
{
	GameStateRet theReturn;
	theReturn.gameState = GameStatesEnum::NO_CHANGE;
	theReturn.seed = 0;	
	this->deltaT = dt;
	if (pauseMenu) {
		pauseUI->update();
		
		if (pauseUI->getButton("continue")->clicked()) {
			pauseMenu = false;
			//disepear mouse
			mouse->activateMouse(true);
			gfx->getWindosClass().HideCoursor();
		}
		if (pauseUI->getButton("menu")->clicked()) {
			theReturn.gameState = GameStatesEnum::TO_MENU;
		}
	}
	if (player->IsAlive()) {

		/*DEBUG*/
		if (keyboard->isKeyPressed(VK_RETURN) && DEBUGMODE) {
			ghost->setActive();
		}
		/*******/
		if (testTime > 0.0f)
		{
			testTime -= dt;
		}
		/*Move things*/
		camera->updateCamera(dt);
		for (int i = 0; i < billboardGroups.size(); i++) {
			billboardGroups[i]->update(dt, gfx);
		}
		camera->calcFURVectors();
		skybox->update(camera->getPos());

		/*update matrixes*/
		collisionHandler.update();

		GameObjManager->updateMatrix();

		/*update vertex*/
		updateShaders();

		/*update things*/
		soundManager.update(camera->getPos(), camera->getForwardVec(), dt);

		GameObjManager->update(dt);

		this->powerupManager->update();

		/*update things*/
		soundManager.update(camera->getPos(), camera->getForwardVec());
		gfx->Update(dt, camera->getPos());
		HUD->UpdateGhostBar(player->getPos(), generationManager->getPuzzelPos(), ghost->getPos(), distanceFromStartPosToPuzzle);
		//HUD->UpdateScore(player->GetScore());
		//let the two lights follow player
		light[0]->getPos().x = player->getPos().x;
		light[0]->getPos().y = player->getPos().y;
		light[0]->getPos().z = player->getPos().z;

		light[1]->getPos().x = player->getPos().x;
		light[1]->getPos().z = player->getPos().z;

		puzzleManager->UpdatePlayerPosition(this->player->getPos());

		Interact(this->GameObjManager->getAllInteractGameObjects());

	}
	else {//player !alive
		soundManager.update(camera->getPos(), camera->getForwardVec());
		if (!player->GetSubmitName()) {
			UI->getStringElement("NameDesc")->setPosition(vec2(-0.9f, 0.3f));
			UI->getStringElement("NameDesc2")->setPosition(vec2(-0.9f, 0.15f));
			UI->getStringElement("Name")->setPosition(vec2(-0.5f, -0.2f));
			player->SetSubmitName(true);
		}
		if (keyboard->isKeyPressed(VK_RETURN)) {
			player->writeScore();
			player->ResetName();
			keyboard->onKeyReleased(VK_RETURN);
			theReturn.gameState = GameStatesEnum::TO_MENU;
		}
		else{
			SetName();
		}
	}

	return theReturn;
}

void Game::render()
{
	gfx->setRenderTarget();
	gfx->setTransparant(true);
	gfx->get_IMctx()->PSSetShaderResources(1, 1, &shadowMap->GetshadowResV());//add ShadowMapping
	this->DrawToBuffer();
	this->ForwardDraw();
	
	gfx->present(this->lightNr);	
}

void Game::updateShaders(bool vs, bool ps)
{

	for (int i = 0; i < billboardGroups.size(); i++) {//TODO: REMOVE?
		billboardGroups[i]->updateShader(gfx, camera->getPos());
	}
	if (vs)
	{
		GameObjManager->updateVertex();
		skybox->updateVertexShader(gfx);
		player->updateVertexShader(gfx);
	}
	if (ps) {
		GameObjManager->updatePixel();
		skybox->updatePixelShader(gfx);
		player->updatePixelShader(gfx);
	}
}

void Game::ForwardDraw()
{
	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[1]);
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[1], nullptr, 0);
	gfx->get_IMctx()->GSSetShader(gfx->getGS()[0], nullptr, 0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[1], nullptr, 0);
	for (int i = 0; i < billboardGroups.size(); i++) {
		billboardGroups[i]->draw(gfx);
	}
}

void Game::DrawToBuffer()
{
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[0]);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);

	skybox->draw(gfx);

	gfx->get_IMctx()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	puzzleManager->Update(this->deltaT); 
	generationManager->draw(); //Todo: ask Simon where to put this...
	GameObjManager->draw();
	camera->calcFURVectors();

	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[0], nullptr, 0);
	letter3DHandler->draw();
	if (pauseMenu && player->IsAlive()) {
		pauseUI->draw();
	}
	if (player->IsAlive())
	{
		HUD->Update();
	}
	UI->draw();

}

void Game::setUpObject()
{
	////////OBJECTS///////////

	player = new Player(rm->get_Models("Player.obj", gfx), gfx, camera, mouse, keyboard, HUD, vec3(0.0f, 0.0f, 0.0f),vec3(0,0,0), vec3(0.2f,0.7f,0.2f));
	player->getSoundManager(soundManager);
	GameObjManager->addGameObject(player, "Player");
	collisionHandler.addPlayer(player);
	generationManager->set_player(player);

	ghost = new Ghost(player, rm->get_Models("ghost.obj", gfx), gfx, player->getPos() - vec3(0, 0, -30), vec3(0, 0, 0), vec3(0.2f, 0.2f, 0.2f));
	ghost->getSoundManager(soundManager);
	GameObjManager->addGameObject(ghost, "Ghost");
	collisionHandler.addEnemies(ghost);

	powerupManager->init(player, ghost);
	enemyManager->init(player, ghost);

	GameObjManager->CreateGameObject("Pearl.obj", "playerPearl", vec3(1000.0f, 1000.0f, 1000.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.05f, 0.05f, 0.05f));
	collisionHandler.addPearl(GameObjManager->getGameObject("playerPearl"));
	player->SetPearl(GameObjManager->getGameObject("playerPearl"));

	generationManager->initialize();
	//generationManager->initialize(); //NOTE: this should be done later, but is currently activated through IMGUI widget
	distanceFromStartPosToPuzzle = generationManager->getPuzzelPos().length();

	std::string skyboxTextures[6] = {
		"assets/textures/Skybox/posx.png",//x+
		"assets/textures/Skybox/negx.png",//x-
		"assets/textures/Skybox/posy.png",//y+
		"assets/textures/Skybox/negy.png",//y-
		"assets/textures/Skybox/posz.png",//z+
		"assets/textures/Skybox/negz.png"//z-
	};
	skybox = new SkyBox(rm->get_Models("skybox_cube.obj", gfx), gfx, player->getPos(), rm->getSpriteCube(skyboxTextures,gfx));
}

void Game::setUpLights()
{
	//current max number is set in graphics.cpp and transforms.hlsli
	nrOfLight = 3;
	light = new Light * [nrOfLight];

	//create the lights with 
	light[0] = new PointLight(vec3(3, 25, 5), 0.5, vec3(1, 1, 1));
	light[1] = new DirLight(vec3(
		0,
		200,
		0),
		vec3(0, -1.57f, 1),
		100,
		100
	);
	
	light[2] = new PointLight(generationManager->getPuzzelPos() + vec3(0, 10, 0), 10, vec3(0.5, 0.5, 0));

	//set color for lights (deafault white)
	light[0]->getColor() = vec3(1, 0, 0);
	light[1]->getColor() = vec3(0.27f/3.f, 0.97f/3.f, 0.97f/3.f);
	//say to graphics/shaders how many lights we have
	gfx->getLightconstbufferforCS()->nrOfLights.element = nrOfLight;
	gfx->takeLight(light, nrOfLight);
}

void Game::setUpParticles()
{
	//add the billboards here
	billboardGroups.push_back(new BillBoardGroup(gfx, rm->getFire(), 1, vec3(0, 0, 0), vec3(5, 5, 5)));

	//if billboard have animation add it here
	billboardGroups[0]->setAnimation(6, 1, 0.16f);
}

void Game::setUpUI()
{
	UI = new UIManager(rm, gfx);
	
	//Name Input
	UI->createUIString("Write your name and", vec2(-10.0f, 0.3f), vec2(0.08f, 0.08f), "NameDesc");
	UI->createUIString("press Enter to submit!", vec2(-10.0f, 0.15f), vec2(0.08f, 0.08f), "NameDesc2");
	UI->createUIString(player->GetName(), vec2(-10.0f, -0.2f), vec2(0.1f, 0.1f), "Name");

	//pause UI
	pauseUI = new UIManager(rm, gfx);
	pauseUI->createUIButton("assets/textures/buttonBack.png", " continue ", mouse, vec2(-0.75f, -0.2f), vec2(0.5f, 0.3f), "continue", vec2(0,0.05f), vec2(0,0.1f));
	pauseUI->createUIButton("assets/textures/buttonBack.png", " main menu ", mouse, vec2(0.25f, -0.2f), vec2(0.5f, 0.3f), "menu", vec2(0, 0.05f), vec2(0,0.1f));
	pauseUI->createUIString("Game Menu", vec2(-0.5f,0.3f), vec2(1/9.f,0.5f), "Game Menu");
}

void Game::setUpSound()
{
	soundManager.loadSound("assets/audio/ah.wav", 5, "ah1");
	soundManager.loadSound("assets/audio/Goat.wav", 5, "Goat");
	soundManager.loadSound("assets/audio/Portal7.wav", 10, "Portal");
	soundManager.loadSound("assets/audio/Powerup6.wav", 10, "Pickup");
	soundManager.loadSound("assets/audio/Jump4.wav", 3, "Jump");
	soundManager.loadSound("assets/audio/Jump2.wav", 60, "Bounce");
	soundManager.loadSound("assets/audio/Land4.wav", 30, "Land");
	soundManager.loadSound("assets/audio/Fall1.wav", 20, "Scream");
	soundManager.loadSound("assets/audio/Shoved2.wav", 25, "Shoved");
	soundManager.loadSound("assets/audio/game_over.wav", 10, "GameOver");
	soundManager.loadSound("assets/audio/begin.wav", 10, "Start");
	soundManager.loadSound("assets/audio/Correct2.wav", 15, "Correct");
	soundManager.loadSound("assets/audio/Wrong4.wav", 15, "Wrong");
	soundManager.loadSound("assets/audio/Powerup7.wav", 10, "GoldApple");
	soundManager.loadSound("assets/audio/Freeze1.wav", 10, "Freeze");
	soundManager.loadSound("assets/audio/Farts.wav", 30, "Rocket");
	soundManager.loadSound("assets/audio/EMP2.wav", 10, "EMP");
	soundManager.loadSound("assets/audio/Pearl2.wav", 10, "Pearl");
	soundManager.loadSound("assets/audio/Jump2.wav", 10, "Pad");
	soundManager.loadSound("assets/audio/Feather1.wav", 10, "Feather");
	soundManager.loadSound("assets/audio/Potion1.wav", 10, "Potion");
	soundManager.loadSound("assets/audio/Shield1.wav", 10, "Shield");
	soundManager.loadSound("assets/audio/Shield2.wav", 10, "Shield2");
	soundManager.loadSound("assets/audio/Coin1.wav", 10, "Money");
	soundManager.loadSound("assets/audio/Hit2.wav", 70, "Hit");
	soundManager.loadSound("assets/audio/RumbleFade.wav", 20, "Rumble");
	soundManager.loadSound("assets/audio/wind1.wav", 0, "Wind");
	soundManager.loadSound("assets/audio/sci-fi-gun-shot.wav", 10, "TurrShot");
	soundManager.loadSound("assets/audio/HeartBeatSimon.wav", 25, "HeartBeat");
	soundManager.loadSound("assets/audio/EpicBeat.wav", 5.0f, "MusicBase");
	soundManager.loadSound("assets/audio/MusicChange.wav", 0.0f, "MusicChange");
	soundManager.playSound("MusicBase", player->getPos());
	soundManager.playSound("MusicChange", player->getPos());
	soundManager.setLoopSound("MusicBase", true);
	soundManager.setLoopSound("MusicChange", true);

	soundManager.playSound("Start", player->getPos());
	soundManager.playSound("Wind", player->getPos());

	soundManager.setLoopSound("Wind", true);
}

void Game::Interact(std::vector<GameObject*>& interactables)
{
	float rayDist;
	float rayDistTemp;
	float maxDist = 10.0f;
	vec3 objMidPos;
	int toInteractIndex = -1;
	vec3 toInteractVec = vec3{ 0.0, 0.0f, 0.0f };
	bool interact = false;
	float size = 0.0f;
	for (int i = 0; i < interactables.size(); i++) {
		
		objMidPos = GetMidPos(interactables[i], size);
		
		//RayDist is the shortest path from the center of the object to the nearest point on the ray
		if (CanInteract(camera->getPos(), camera->getForwardVec(), objMidPos, size / 2, maxDist, rayDistTemp)) {
			if (toInteractIndex == -1) {
				toInteractIndex = i;
				toInteractVec = objMidPos;
				rayDist = rayDistTemp;
			}
			else {
				float l1 = (camera->getPos() - objMidPos).length();
				float l2 = (camera->getPos() - toInteractVec).length();
				if ((l1 + rayDistTemp * (l1 / (maxDist / 3))) < (l2 + rayDist * (l1 / (maxDist / 3)))) {
					toInteractIndex = i;
					toInteractVec = objMidPos;
					rayDist = rayDistTemp;
				}
			}
			if (!interact)
				interact = true;
		}
	}
	if (interact) {
		if (!interactables[toInteractIndex]->isUsed()) {
			if (mouse->IsLeftDown()) {
				//std::cout << "Interact!\n";
				interactables[toInteractIndex]->Use();
				interactables[toInteractIndex]->addScale(vec3(0.1f, 0.1f, 0.1f));
			}	
		}
		else {
			if (mouse->isRightDown()) {
				//std::cout << "Un-interact!\n";
				interactables[toInteractIndex]->Use();
				interactables[toInteractIndex]->addScale(vec3(-0.1f, -0.1f, -0.1f));
			}	
		}
	}

	if (mouse->IsLeftDown() && testTime <= 0.0f)
	{
		testTime = 1.0f;
		//TODO: REMOVE COMMENT
		puzzleManager->Interact(GameObjManager->getGameObject("Player")->getPos(), camera->getForwardVec());
		if (puzzleManager->isCompleted())
		{
			player->Reset(true);
			generationManager->initialize();
			soundManager.playSound("Portal", player->getPos());
		}
	}
}

void Game::SetName()
{
	//A-Z
	for (int i = 65; i < 91; i++) {
		if (keyboard->isKeyPressed(i)) {
			player->AddToName(i);
			keyboard->onKeyReleased(i);
		}
	}
	//0-9
	for (int i = 48; i < 58; i++) {
		if (keyboard->isKeyPressed(i)) {
			player->AddToName(i);
			keyboard->onKeyReleased(i);
		}
	}
	//Backspace
	if (keyboard->isKeyPressed(VK_BACK)) {
		player->RemoveLetter();
		keyboard->onKeyReleased(VK_BACK);
	}
	//Space
	if (keyboard->isKeyPressed(VK_SPACE)) {
		player->AddToName('_');
		keyboard->onKeyReleased(VK_SPACE);
	}
	UI->getStringElement("Name")->setText(player->GetName());
}

