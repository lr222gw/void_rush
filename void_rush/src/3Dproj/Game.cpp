#include "Game.h"


Game::Game(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam):
	GameState(gfx,rm,imguimanager,mouse,keyboard,cam),
	soundManager(1)//be able to change this later based on settings
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
	testPuzzle = new ProtoPuzzle(gfx, rm, collisionHandler, &soundManager); //TODO: REMOVE COMMENT
	
	generationManager = new Generation_manager(gfx, rm, collisionHandler);
	generationManager->set_PuzzleManager(testPuzzle); //TODO: REMOVE COMMENT
	generationManager->set_GameObjManager(GameObjManager);
	
	camera->setRotation(vec3(0, 0, 0));
	pauseMenu = false;
	
	letter3DHandler = new Letters3DHandler(rm, gfx);

	/*set ups*/
	this->setUpObject();
	this->setUpLights();
	this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx, gfx->getClientWH().x, gfx->getClientWH().y);
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
	for (int i = 0; i < LightVisualizers.size(); i++) {
		delete LightVisualizers[i];
	}
	for (int i = 0; i < nrOfLight; i++) {
		delete light[i];
	}
	delete[] light;
	for (int i = 0; i < billboardGroups.size(); i++) {
		delete billboardGroups[i];
	}
	
	delete skybox;
	delete testPuzzle; //TODO: REMOVE COMMENT
	delete generationManager;
	delete HUD;
	delete UI;
	delete pauseUI;
	delete GameObjManager;
	delete letter3DHandler;
}

void Game::handleEvents()
{
	/*Read Mouse*/
	while (!mouse->EventBufferEmpty() && mouse->getMouseActive()) {
		mouseEvent e = mouse->ReadEvent();
		if (e.getType() == mouseEvent::EventType::RAW_MOVE && !pauseMenu) {
			player->rotateWithMouse(e.getPosX(), e.getPosY());
		}
		if (e.getType() == mouseEvent::EventType::LPress) {

			//soundManager.playSound("ah1", player->getPos());
		}
		if (e.getType() == mouseEvent::EventType::RPress) {

			soundManager.playSound("German", player->getPos());
		}
	}
	if (keyboard->onceisKeyReleased('F') && player->IsAlive()) {
		//set pause
		pauseMenu = !pauseMenu;
		if (pauseMenu) {
			gfx->getWindosClass().ShowCoursor();
		}
		else {
			gfx->getWindosClass().HideCoursor();
		}
	}
	if (keyboard->onceisKeyReleased('N')) {
		soundManager.changeMusic("assets/audio/More_Plastic-Rewind.wav", 7, 4);
	}
	if (keyboard->onceisKeyReleased('M')) {
		soundManager.changeMusic("assets/audio/EpicBeat.wav", 7.0f, 4);
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

GameStatesEnum Game::update(float dt)
{
	GameStatesEnum theReturn = GameStatesEnum::NO_CHANGE;
	if (pauseMenu) {
		pauseUI->update();
		gfx->Update(dt, camera->getPos());
		
		if (pauseUI->getButton("continue")->clicked()) {
			pauseMenu = false;
			//disepear mouse
			mouse->activateMouse(true);
			gfx->getWindosClass().HideCoursor();
		}
		if (pauseUI->getButton("menu")->clicked()) {
			theReturn = GameStatesEnum::TO_MENU;
		}
	}
	if (player->IsAlive()) {

		/*DEBUG*/
		if (keyboard->isKeyPressed(VK_RETURN)) {
			ghost->setActive();
		}
		/*******/
		if (testTime > 0.0f)
		{
			testTime -= dt;
		}
		/*Move things*/
		camera->updateCamera(dt);
		if (getkey('N')) {
			DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-GameObjManager->getGameObject(1)->getPos().x, -GameObjManager->getGameObject(1)->getPos().y, -GameObjManager->getGameObject(1)->getPos().z, 1.0f
			);
			XRotation(viewMatrix, GameObjManager->getGameObject(1)->getRot().x);
			YRotation(viewMatrix, GameObjManager->getGameObject(1)->getRot().y);
			gfx->getVertexconstbuffer()->view.element = viewMatrix;
		}
		for (int i = 0; i < billboardGroups.size(); i++) {
			billboardGroups[i]->update(dt, gfx);
		}
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->setPos(light[i]->getPos());
			LightVisualizers[i]->setRot(vec3(0, light[i]->getRotation().x, -light[i]->getRotation().y) + vec3(0, 1.57f, 0));
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
		gfx->Update(dt, camera->getPos());

		GameObjManager->update(dt);
		

	/*update things*/
	soundManager.update(camera->getPos(), camera->getForwardVec());
	gfx->Update(dt, camera->getPos());
	HUD->UpdateGhostBar(player->getPos(), generationManager->getPuzzelPos(), ghost->getPos(), distanceFromStartPosToPuzzle);

#pragma region camera_settings

	if (getkey('C')) {
		camera->setPosition(light[lightNr]->getPos());
		camera->setRotation(light[lightNr]->getRotation());
	}
	if (getkey('1') && getkey(VK_F1)) {
		lightNr = 0;
	}
	if (getkey('2') && getkey(VK_F1)) {
		lightNr = 1;
	}
	if (getkey('3') && getkey(VK_F1)) {
		lightNr = 2;
	}
	if (getkey('4') && getkey(VK_F1)) {
		lightNr = 3;
	}

#pragma endregion camera_settings

	
	if (getkey('L'))
	{
		HUD->UpdateScore(100);
	}

	if (getkey('V') && testTime <= 0.0f)
	{
		testInt++;
		if (testInt == 7)
		{
			testInt = 0;
		}
		testTime = 0.2f;
		HUD->ChangeCurrentPowerUp(testInt);
	}

	if (getkey('B') && testTime <= 0.0f)
	{
		testTime = 0.2f;
		if (HUD->GetStatusOfPassive(1))
		{
			HUD->TurnOffPassive(1);
			HUD->TurnOffPassive(2);
			HUD->TurnOffPassive(3);
			HUD->TurnOffPassive(4);
		}
		else
		{
			HUD->TurnOnPassive(1);
			HUD->TurnOnPassive(2);
			HUD->TurnOnPassive(3);
			HUD->TurnOnPassive(4);
		}
	}
	

		testPuzzle->UpdatePlayerPosition(this->player->getPos());

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
			theReturn = GameStatesEnum::TO_MENU;
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
	//if (def_rend) {
	//	//deferred rendering
	//	defRend->BindFirstPass();
	//	this->DrawToBuffer();
	//	defRend->BindSecondPass(shadowMap->GetshadowResV());
	//}
	if (!def_rend) {
		gfx->get_IMctx()->PSSetShaderResources(1, 1, &shadowMap->GetshadowResV());//add ShadowMapping
		this->DrawToBuffer();

	}
	this->ForwardDraw();
	
	gfx->present(this->lightNr);	
}

void Game::updateShaders(bool vs, bool ps)
{

	for (int i = 0; i < billboardGroups.size(); i++) {
		billboardGroups[i]->updateShader(gfx, camera->getPos());
	}
	if (vs)
	{
		GameObjManager->updateVertex();
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->updateVertexShader(gfx);
		}
		skybox->updateVertexShader(gfx);
		player->updateVertexShader(gfx);
	}
	if (ps) {
		GameObjManager->updatePixel();
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->updatePixelShader(gfx);
		}
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
	testPuzzle->Update(); 
	generationManager->draw(); //Todo: ask Simon where to put this...
	GameObjManager->draw();
	camera->calcFURVectors();

	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[0], nullptr, 0);

	if (getkey('F')) {
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->draw(gfx, false);
		}
	}
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

	player = new Player(rm->get_Models("DCube.obj", gfx), gfx, camera, mouse, keyboard, HUD, vec3(0.0f, 0.0f, 0.0f),vec3(0,0,0), vec3(0.2,0.2,0.2));
	player->getSoundManager(soundManager);
	GameObjManager->addGameObject(player, "Player");
	collisionHandler.addPlayer(player);
	generationManager->set_player(player);

	GameObjManager->CreateGameObject("DCube.obj", "cam", vec3(5, -10, 0), vec3(0, 0, 0));
	GameObjManager->CreateGameObject("DCube.obj", "cubetest", vec3(0, 0, 50), vec3(0, 0, 0));

	ghost = new Ghost(player, rm->get_Models("indoor_plant_02.obj", gfx), gfx, player->getPos() - vec3(0, 0, -5), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2));
	ghost->getSoundManager(soundManager);
	GameObjManager->addGameObject(ghost, "Ghost");
	collisionHandler.addEnemies(ghost);

	///////POWERUPS///////
	powers.push_back(new Powerups(rm->get_Models("GoldenApple.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), APPLE));
	GameObjManager->addGameObject(powers.back(), "Apple");
	collisionHandler.addPowerups(powers.back());

	powers.push_back(new Powerups(rm->get_Models("Feather.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), FEATHER));
	GameObjManager->addGameObject(powers.back(), "Feather");
	collisionHandler.addPowerups(powers.back());

	powers.push_back(new Powerups(rm->get_Models("Potion.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), POTION));
	GameObjManager->addGameObject(powers.back(), "Potion");
	collisionHandler.addPowerups(powers.back());

	powers.push_back(new Powerups(rm->get_Models("GoldenApple.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), SHIELD));
	GameObjManager->addGameObject(powers.back(), "Shield");
	collisionHandler.addPowerups(powers.back());

	powers.push_back(new Powerups(rm->get_Models("GoldenApple.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), MONEY));
	GameObjManager->addGameObject(powers.back(), "Money");
	collisionHandler.addPowerups(powers.back());

	powers.push_back(new Powerups(rm->get_Models("GoldenApple.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), FREEZE));
	GameObjManager->addGameObject(powers.back(), "Freeze");
	collisionHandler.addPowerups(powers.back());

	powers.push_back(new Powerups(rm->get_Models("Pearl.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), PEARL));
	GameObjManager->addGameObject(powers.back(), "Pearl");
	collisionHandler.addPowerups(powers.back());

	powers.push_back(new Powerups(rm->get_Models("EMP.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), EMP));
	GameObjManager->addGameObject(powers.back(), "Emp");
	collisionHandler.addPowerups(powers.back());

	powers.push_back(new Powerups(rm->get_Models("GoldenApple.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), PAD));
	GameObjManager->addGameObject(powers.back(), "Pad");
	collisionHandler.addPowerups(powers.back());

	powers.push_back(new Powerups(rm->get_Models("GoldenApple.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), KILL));
	GameObjManager->addGameObject(powers.back(), "Kill");
	collisionHandler.addPowerups(powers.back());

	powers.push_back(new Powerups(rm->get_Models("GoldenApple.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), ROCKET));
	GameObjManager->addGameObject(powers.back(), "Rocket");
	collisionHandler.addPowerups(powers.back());

	powers.push_back(new Powerups(rm->get_Models("GoldenApple.obj", gfx), gfx, player, ghost, keyboard, vec3(1000, 1000, 1000), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), CARD));
	GameObjManager->addGameObject(powers.back(), "Card");
	collisionHandler.addPowerups(powers.back());

	generationManager->initialize();
	testPuzzle->Initiate(generationManager->getPuzzelPos()); 
	//generationManager->initialize(); //NOTE: this should be done later, but is currently activated through IMGUI widget
	distanceFromStartPosToPuzzle = generationManager->getPuzzelPos().length();
	setUpPowerups(1, vec3(10, 10, 10));
	//setUpPowerups(1, vec3(15, 10, 15));
	//setUpPowerups(1, vec3(20, 10, 20));

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
	nrOfLight = 2;
	light = new Light * [nrOfLight];

	//create the lights with 
	//light[0] = new DirLight(vec3(0, 30, 8), vec3(0.1f, -PI / 2, 1.f), 100, 100);
	light[0] = new PointLight(vec3(3, 25, 5), 20, vec3(1, 1, 1));
	//light[1] = new SpotLight(vec3(0, 46, 45), vec3(0, -1.57, 1));
	light[1] = new SpotLight(vec3(0, 500, 0), vec3(0, -1.57, 1));
	//light[2] = new SpotLight(vec3(8, 47.f, 0), vec3(0, -1, 1));
	//light[3] = new SpotLight(vec3(30, 50, 0), vec3(-1, -1, 1));

	//set color for lights (deafault white)
	light[0]->getColor() = vec3(1, 0, 0);
	light[1]->getColor() = vec3(1, 0, 1);

	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers.push_back(new GameObject(rm->get_Models("Camera.obj"), gfx, light[i]->getPos(), light[i]->getRotation()));
	}
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
	//UI->createUISprite("assets/textures/Fire.png", vec2(-1, 0), vec2(0.5, 0.5));
	//UI->createUIString("string", vec2(0, 0), vec2(0.2, 0.5), "penis");
	
	//Name Input
	UI->createUIString("Write your name and", vec2(-10.0f, 0.3f), vec2(0.08f, 0.08f), "NameDesc");
	UI->createUIString("press Enter to submit!", vec2(-10.0f, 0.15f), vec2(0.08f, 0.08f), "NameDesc2");
	UI->createUIString(player->GetName(), vec2(-10.0f, -0.2f), vec2(0.1f, 0.1f), "Name");

	//pause UI
	pauseUI = new UIManager(rm, gfx);
	pauseUI->createUIButton("assets/textures/buttonBack.png", " continue ", mouse, vec2(-0.75, -0.2), vec2(0.5, 0.3), "continue", vec2(0,0.05), vec2(0,0.1));
	pauseUI->createUIButton("assets/textures/buttonBack.png", " main menu ", mouse, vec2(0.25, -0.2), vec2(0.5, 0.3), "menu", vec2(0, 0.05), vec2(0,0.1));
	pauseUI->createUIString("Game Menu", vec2(-0.5,0.3), vec2(1/9.f,0.5), "Game Menu");
}

void Game::setUpSound()
{
	soundManager.loadSound("assets/audio/ah.wav", 5, "ah1");
	soundManager.loadSound("assets/audio/Goat.wav", 5, "Goat");
	soundManager.loadSound("assets/audio/Portal7.wav", 10, "Portal");
	soundManager.loadSound("assets/audio/Powerup6.wav", 10, "Pickup");
	soundManager.loadSound("assets/audio/Jump4.wav", 3, "Jump");
	soundManager.loadSound("assets/audio/Land4.wav", 30, "Land");
	soundManager.loadSound("assets/audio/Fall1.wav", 30, "Scream");
	soundManager.loadSound("assets/audio/Shoved2.wav", 30, "Shoved");
	soundManager.loadSound("assets/audio/game_over.wav", 10, "GameOver");
	soundManager.loadSound("assets/audio/begin.wav", 10, "Start");
	soundManager.loadSound("assets/audio/Correct2.wav", 15, "Correct");
	soundManager.loadSound("assets/audio/Wrong4.wav", 15, "Wrong");
	soundManager.loadSound("assets/audio/Powerup7.wav", 10, "GoldApple");
	soundManager.loadSound("assets/audio/Freeze1.wav", 10, "Freeze");
	soundManager.loadSound("assets/audio/Portal1.wav", 10, "Rocket");
	soundManager.loadSound("assets/audio/Hit.wav", 20, "Hit");
	soundManager.loadSound("assets/audio/German.wav", 40, "German");
	soundManager.loadSound("assets/audio/Wind1.wav", 0, "Wind");
	soundManager.playMusic("assets/audio/EpicBeat.wav", 7.0f);
	soundManager.setMusicLoop(true);

	soundManager.playSound("Start", player->getPos());
	soundManager.playSound("Wind", player->getPos());
	soundManager.setLoopSound("Wind", true);
}

//Decides which powerups are used this map.
void Game::setUpPowerups(int chosenDiff, vec3 pos)
{	
		int chosenPower = 15;
		//int chosenPower = 1 + (rand() % 100);
		//Difficulty easy
		if (chosenDiff == 1)
		{
			if (chosenPower <= 60)
			{
				//Got F-tier
				if (chosenPower <= 15)
				{
					//choose feather and moved into position
					GameObjManager->getGameObject("Feather")->setPos(pos);

				}
				else if (chosenPower > 15 && chosenPower <= 30)
				{
					//choose speed and moved into position
					GameObjManager->getGameObject("Potion")->setPos(pos);

				}
				else if (chosenPower > 30 &&  chosenPower <= 45)
				{
					//choose shield and moved into position
					GameObjManager->getGameObject("Shield")->setPos(pos);
				}
				else if (chosenPower > 45 && chosenPower <= 60)
				{
					// choose Money and moved into position
					GameObjManager->getGameObject("Money")->setPos(pos);
				}
			}
			else if (chosenPower > 60 && chosenPower <= 92)
			{
				//Got C-tier
				if (chosenPower > 60 && chosenPower <= 68)
				{
					//Freeze
					GameObjManager->getGameObject("Freeze")->setPos(pos);
				}
				else if (chosenPower > 68 && chosenPower <= 76)
				{
					//Pearl
					GameObjManager->getGameObject("Pearl")->setPos(pos);
				}
				else if (chosenPower > 76 && chosenPower <= 84)
				{
					// EMP
					GameObjManager->getGameObject("Emp")->setPos(pos);
				}
				else if (chosenPower > 84 && chosenPower <= 92)
				{
					// PAD
					GameObjManager->getGameObject("Pad")->setPos(pos);
				}
			}
			else if (chosenPower >= 93)
			{
				//Got S-tier
				if (chosenPower >= 93 && chosenPower <= 94)
				{
					//Freeze
					GameObjManager->getGameObject("Apple")->setPos(pos);
				}
				else if (chosenPower > 94 && chosenPower <= 96)
				{
					//Pearl
					GameObjManager->getGameObject("Kill")->setPos(pos);
				}
				else if (chosenPower > 96 && chosenPower <= 98)
				{
					// EMP
					GameObjManager->getGameObject("Rocket")->setPos(pos);
				}
				else if (chosenPower > 98 && chosenPower <= 100)
				{
					// PAD
					GameObjManager->getGameObject("Card")->setPos(pos);
				}
			}
		}
		else if (chosenDiff == 2) //Difficult medium
		{
			if (chosenPower <= 36)
			{
				//Got F-tier
				if (chosenPower <= 9)
				{
					//choose feather and moved into position
					GameObjManager->getGameObject("Feather")->setPos(pos);

				}
				else if (chosenPower > 9 && chosenPower <= 18)
				{
					//choose speed and moved into position
					GameObjManager->getGameObject("Potion")->setPos(pos);

				}
				else if (chosenPower > 18 && chosenPower <= 27)
				{
					//choose shield and moved into position
					GameObjManager->getGameObject("Shield")->setPos(pos);
				}
				else if (chosenPower > 27 && chosenPower <= 36)
				{
					// choose Money and moved into position
					GameObjManager->getGameObject("Money")->setPos(pos);
				}
			}
			else if (chosenPower > 36 && chosenPower <= 84)
			{
				//Got C-tier
				if (chosenPower >= 37 && chosenPower <= 48)
				{
					//Freeze
					GameObjManager->getGameObject("Freeze")->setPos(pos);
				}
				else if (chosenPower > 48 && chosenPower <= 60)
				{
					//Pearl
					GameObjManager->getGameObject("Pearl")->setPos(pos);
				}
				else if (chosenPower > 60 && chosenPower <= 72)
				{
					// EMP
					GameObjManager->getGameObject("Emp")->setPos(pos);
				}
				else if (chosenPower > 72 && chosenPower <= 84)
				{
					// PAD
					GameObjManager->getGameObject("Pad")->setPos(pos);
				}
			}
			else if (chosenPower >= 85)
			{
				//Got S-tier
				if (chosenPower >= 85 && chosenPower <= 88)
				{
					//Freeze
					GameObjManager->getGameObject("Apple")->setPos(pos);
				}
				else if (chosenPower > 88 && chosenPower <= 92)
				{
					//Pearl
					GameObjManager->getGameObject("Kill")->setPos(pos);
				}
				else if (chosenPower > 92 && chosenPower <= 96)
				{
					// EMP
					GameObjManager->getGameObject("Rocket")->setPos(pos);
				}
				else if (chosenPower > 96 && chosenPower <= 100)
				{
					// PAD
					GameObjManager->getGameObject("Card")->setPos(pos);
				}
			}
		}
		else if (chosenDiff == 3)
		{
			if (chosenPower <= 24)
			{
				//Got F-tier
				if (chosenPower <= 6)
				{
					//choose feather and moved into position
					GameObjManager->getGameObject("Feather")->setPos(pos);

				}
				else if (chosenPower > 6  && chosenPower <= 12)
				{
					//choose speed and moved into position
					GameObjManager->getGameObject("Potion")->setPos(pos);

				}
				else if (chosenPower > 12 && chosenPower <= 18)
				{
					//choose shield and moved into position
					GameObjManager->getGameObject("Shield")->setPos(pos);
				}
				else if (chosenPower > 18 && chosenPower <= 24)
				{
					// choose Money and moved into position
					GameObjManager->getGameObject("Money")->setPos(pos);
				}
			}
			else if (chosenPower > 24 && chosenPower <= 60)
			{
				//Got C-tier
				if (chosenPower >= 24  && chosenPower <= 33)
				{
					//Freeze
					GameObjManager->getGameObject("Freeze")->setPos(pos);
				}
				else if (chosenPower > 33 && chosenPower <= 42)
				{
					//Pearl
					GameObjManager->getGameObject("Pearl")->setPos(pos);
				}
				else if (chosenPower > 42 && chosenPower <= 51)
				{
					// EMP
					GameObjManager->getGameObject("Emp")->setPos(pos);
				}
				else if (chosenPower > 51 && chosenPower <= 60)
				{
					// PAD
					GameObjManager->getGameObject("Pad")->setPos(pos);
				}
			}
			else if (chosenPower > 61)
			{
				//Got S-tier
				if (chosenPower >= 61 && chosenPower <= 70)
				{
					//Freeze
					GameObjManager->getGameObject("Apple")->setPos(pos);
				}
				else if (chosenPower > 70 && chosenPower <= 80)
				{
					//Pearl
					GameObjManager->getGameObject("Kill")->setPos(pos);
				}
				else if (chosenPower > 80 && chosenPower <= 90)
				{
					// EMP
					GameObjManager->getGameObject("Rocket")->setPos(pos);
				}
				else if (chosenPower > 90 && chosenPower <= 100)
				{
					// PAD
					GameObjManager->getGameObject("Card")->setPos(pos);
				}
			}
		}
		std::cout << chosenPower << std::endl;
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
		testPuzzle->Interact(GameObjManager->getGameObject("Player")->getPos(), camera->getForwardVec());
		if (testPuzzle->isCompleted())
		{
			//player->setPos(vec3(0.0f, 0.0f, 0.0f));
			player->Reset(true);
			generationManager->initialize();
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

