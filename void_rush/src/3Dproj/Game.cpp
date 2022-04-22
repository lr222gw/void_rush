#include "Game.h"

Game::Game(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow):
	soundManager(1)
{
	gfx = new Graphics(hInstance, hPrevInstance, lpCmdLine, nCmdShow, mouse);
	mouse = gfx->getWindosClass().getMouse();
	keyboard = gfx->getWindosClass().getKeyboard();

	defRend = new DeferredRendering(gfx);
	//Create a buffer for the light const buffer(hlsli)
	CreateConstBuffer(gfx, gfx->getConstBuffers(0), sizeof(*gfx->getLightconstbufferforCS()), gfx->getLightconstbufferforCS());
	CreateConstBuffer(gfx, gfx->getConstBuffers(1), sizeof(*gfx->getCamPosconstbuffer()), gfx->getCamPosconstbuffer());
	//Resource manager
	rm = new ResourceManager(gfx);
	
	setUpUI();
	testPuzzle = new ProtoPuzzle(gfx, rm, collisionHandler);

	generationManager = new Generation_manager(gfx,rm, collisionHandler);
	generationManager->set_PuzzleManager(testPuzzle);
	

	setUpLights();
	
	//shadow map needs to take more lights
	this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx, 1920U, 1080U);
	//this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx, 640u, 360U);
	
	gfx->takeIM(&this->IMGUIManager);
	this->IMGUIManager.set_owner(this);
	
	camera = new Camera(gfx, mouse, vec3(0.0f,0.0f,0.0f), vec3(0.0f,0.0f,0.0f));
	camera->setData();
	setUpObject();
	
	

	setUpParticles();
	if (IMGUI) {
		//UIManager.takeObject(obj[2]);
		//UIManager.takeObject(obj[3]);
		/*IMGUI*/
		for (int i = 0; i < nrOfLight; i++) {
			IMGUIManager.takeLight(light[i]);
		}
	}
	
	
	
	lightNr = 0;
	//soundManager.playMusic("assets/audio/More_Plastic-Rewind.wav");
	soundManager.loadSound("assets/audio/ah.wav", 5, "ah1");
	//soundManager.loadSound("assets/audio/ah2.wav", 1, "ah2");
	//soundManager.loadSound("assets/audio/moh.wav", 1, "moh");
	//soundManager.loadSound("assets/audio/oh1.wav", 1, "oh1");
	

	std::string skyboxTextures[6] = {
		"assets/textures/Skybox/sky_stars_01bk.png",//back
		"assets/textures/Skybox/sky_stars_01dn.png",//down
		"assets/textures/Skybox/sky_stars_01ft.png",//front
		"assets/textures/Skybox/sky_stars_01lf.png",//left
		"assets/textures/Skybox/sky_stars_01rt.png",//right
		"assets/textures/Skybox/sky_stars_01up.png",//up
	};
	Space = new SkyBox(rm->get_Models("skybox_cube.obj", gfx), gfx, player->getPos(), skyboxTextures);

	maxDepth = -140.0f;

}

Game::~Game() 
{
	//part of game
 	TC::GetInst().empty();
	delete gfx;
	delete rm;

	//logic and other
	delete defRend;
	delete camera;
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
	delete Space;
	delete testPuzzle;
	delete generationManager;
	delete UI;
	delete GameObjManager;
}


void Game::run()
{
static bool once = false;

while (msg.message != WM_QUIT && gfx->getWindosClass().ProcessMessages())
{
	if (dt.dt() > 0.2f) {
		dt.setDeltaTime(0.2f);
	}
	if (keyboard->isKeyPressed('P')) {
		gfx->getWindosClass().HideCoursor();
	}
	else if (keyboard->isKeyPressed('O')) {
		gfx->getWindosClass().ShowCoursor();
	}
	/*Read Mouse*/
	while (!mouse->EventBufferEmpty() && mouse->getMouseActive()) {
		mouseEvent e = mouse->ReadEvent();
		if (e.getType() == mouseEvent::EventType::RAW_MOVE) {
			player->rotateWithMouse(e.getPosX(), e.getPosY());
		}
		static int os = 0;
		if (e.getType() == mouseEvent::EventType::LPress) {

			soundManager.playSound("ah1", GameObjManager->getGameObject(0)->getPos());
		}
	}
	gfx->clearScreen();
	gfx->setTransparant(false);
	//for shadow
	//måste uppdatera detta så inte hela object uppdateras när bara skugga ska
	shadowMap->setUpdateShadow();
	vec3 camLP = camera->getPos();
	vec3 camLR = camera->getRot();
	for (int i = 0; i < nrOfLight; i++) {
		//set cam position to lightposition
		camera->setPosition(light[i]->getPos());
		camera->setRotation(light[i]->getRotation());
		shadowMap->inUpdateShadow(i);
		updateShaders(true, false);
		DrawAllShadowObject();
	}
	//set cam position so its the real cam
	camera->setPosition(camLP);
	camera->setRotation(camLR);
	gfx->setProjection(0);//last can be dir light
	gfx->RsetViewPort();

	Update();
	if (def_rend) {
		//deferred rendering
		defRend->BindFirstPass();
		this->DrawToBuffer();
		defRend->BindSecondPass(shadowMap->GetshadowResV());
	}

	gfx->setRenderTarget();
	gfx->setTransparant(true);
	if (!def_rend) {
		//if deferred rendfering 
		gfx->get_IMctx()->PSSetShaderResources(1, 1, &shadowMap->GetshadowResV());//add ShadowMapping
		this->DrawToBuffer();
	}
	this->ForwardDraw();
	gfx->present(this->lightNr);

	//once = false;
	dt.restartClock();

	if (!player->IsAlive()) {
		break;
	}
}
printf("quit");
}

void Game::Update()
{
	/*DEBUG*/
	if (keyboard->isKeyPressed(VK_RETURN)) {
		ghost->setActive();
	}
	/*******/
	if (testTime > 0.0f)
	{
		testTime -= (float)dt.dt();
	}
	/*Move things*/
	camera->updateCamera((float)dt.dt());
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
		billboardGroups[i]->update((float)dt.dt(), gfx);
	}
	for (int i = 0; i < LightVisualizers.size(); i++) {
		LightVisualizers[i]->setPos(light[i]->getPos());
		LightVisualizers[i]->setRot(vec3(0, light[i]->getRotation().x, -light[i]->getRotation().y) + vec3(0, 1.57f, 0));
	}
	camera->calcFURVectors();
	Space->update(camera->getPos());

	/*update matrixes*/
	GameObjManager->updateMatrix();
	player->updateMatrix();

	/*Collision checking*/
	//collisionWithBlocking(player->getPlayerObjPointer(), GameObjManager->getGameObject("Ground"));
	//collisionWithBlocking(player->getPlayerObjPointer(), obj[2]);
	//generationManager->updatePlatfoms();

	
	collisionHandler.update();

	/*update vertex*/
	updateShaders();

	/*update things*/
	soundManager.update(camera->getPos(), camera->getForwardVec());
	gfx->Update((float)dt.dt(), camera->getPos());
	GameObjManager->update(dt.dt());
	player->update((float)dt.dt());

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


	/*Interaction testing*/
	//interactTest(this->GameObjManager->getAllGameObjects());
	//std::vector<GameObject*> temp = this->GameObjManager->getAllGameObjects();
	//std::vector<GameObject*> temp2 = {temp[1],temp[2],temp[3], temp[0]};

	Interact(this->GameObjManager->getAllInteractGameObjects());
	HandlePlayer();
	
}


void Game::DrawToBuffer()
{	
	
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[0]);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);
	
	Space->draw(gfx);
	
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	testPuzzle->Update();
	player->draw(gfx);
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
	//gfx->get_IMctx()->OMSetRenderTargets(1, &gfx->getRenderTarget(), nullptr);
	UI->draw();
	//gfx->get_IMctx()->OMSetRenderTargets(1, &gfx->getRenderTarget(), gfx->getDepthStencil());
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

void Game::DrawAllShadowObject()
{
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->PSSetShader(nullptr, nullptr, 0);
	GameObjManager->drawShadow();
}

void Game::updateShaders(bool vs, bool ps)
{
	for (int i = 0; i < billboardGroups.size(); i++) {
		billboardGroups[0]->updateShader(gfx, camera->getPos());
	}
	
	if (vs)
	{
		GameObjManager->updateVertex();
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->updateVertexShader(gfx);
		}
		Space->updateVertexShader(gfx);
		player->updateVertexShader(gfx);
	}
	if (ps) {
		GameObjManager->updatePixel();
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->updatePixelShader(gfx);
		}
		Space->updatePixelShader(gfx);
		player->updatePixelShader(gfx);
	}
}


void Game::setUpObject()
{
	GameObjManager = new GameObjectManager(gfx, rm);
	////////OBJECTS///////////
	//GameObjManager->CreateGameObject("Camera.obj", "Camera1", vec3(0.f, 0.f, 10.f), vec3(0.f, 0.f, 0.f), vec3(5.f, 5.0f, 5.0f)); //main
	//GameObjManager->CreateGameObject("Camera.obj", "Camera2", vec3(0.f, 100.f, 0.f), vec3(0.f, -1.58f, 0.f), vec3(2.f, 2.0f, 2.0f)); //main

	//GameObjManager->CreateGameObject("quad2.obj", "Ground", vec3(0, -5, 0), vec3(0, 0, 1.57f), vec3(100, 100, 100));
	
	//GameObjManager->CreateGameObject("BasePlatform.obj", "Base", vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));
	//GameObjManager->CreateGameObject("nanosuit.obj","Nano", vec3(-5.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));


	
	player = new Player(rm->get_Models("DCube.obj", gfx), gfx, camera, mouse, keyboard, vec3(0.0f, 0.0f, 0.0f));
	GameObjManager->addGameObject(player, "Player");
	collisionHandler.addPlayer(player);
	generationManager->set_player(player);
	//collisionHandler.addPlatform(GameObjManager->getGameObject("Ground"));

	ghost = new Ghost(player, rm->get_Models("indoor_plant_02.obj",gfx), gfx, player->getPos() - vec3(0, 0, -5),vec3(0,0,0), vec3(0.2,0.2,0.2));
	GameObjManager->addGameObject(ghost, "Ghost");
	collisionHandler.addEnemies(ghost);

	generationManager->initialize();
	testPuzzle->Initiate(generationManager->getPuzzelPos());
	//generationManager->initialize(); //NOTE: this should be done later, but is currently activated through IMGUI widget


	//INteraction
	/*GameObjManager->CreateGameObject("indoor_plant_02.obj", "Test1", vec3(0, 5, 30), vec3(0, 0, 0), vec3(1, 1, 1));
	GameObjManager->CreateGameObject("indoor_plant_02.obj", "Test2", vec3(10, 5, 30), vec3(0, 0, 0), vec3(1, 1, 1));
	GameObjManager->CreateGameObject("indoor_plant_02.obj", "Test3", vec3(20, 5, 30), vec3(0, 0, 0), vec3(1, 1, 1));
	GameObjManager->addInteractGameObject(GameObjManager->getGameObject("Test1"));
	GameObjManager->addInteractGameObject(GameObjManager->getGameObject("Test2"));
	GameObjManager->addInteractGameObject(GameObjManager->getGameObject("Test3"));*/
}

void Game::setUpLights()
{
	//current max number is set in graphics.cpp and transforms.hlsli
	nrOfLight = 1;
	light = new Light * [nrOfLight];

	//create the lights with 
	//light[0] = new DirLight(vec3(0, 30, 8), vec3(0.1f, -PI / 2, 1.f), 100, 100);
	light[0] = new PointLight(vec3(3, 25, 5), 200, vec3(1,1,1));
	//light[1] = new SpotLight(vec3(18, 46, 45), vec3(-2.4f, -0.5, 1));
	//light[2] = new SpotLight(vec3(8, 47.f, 0), vec3(0, -1, 1));
	//light[3] = new SpotLight(vec3(30, 50, 0), vec3(-1, -1, 1));

	//set color for lights (deafault white)
	light[0]->getColor() = vec3(1, 1, 1);
	//light[1]->getColor() = vec3(1, 0, 1);

	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers.push_back(new GameObject(rm->get_Models("Camera.obj"),gfx, light[i]->getPos(), light[i]->getRotation()));
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
}

/*Interaction Test*/
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
				if ((l1 + rayDistTemp*(l1/(maxDist/3))) < (l2 + rayDist* (l1 / (maxDist / 3)))) {
					toInteractIndex = i;
					toInteractVec = objMidPos;
					rayDist = rayDistTemp;
				}
			}
			if(!interact)
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
		testPuzzle->Interact(GameObjManager->getGameObject("Player")->getPos(), camera->getForwardVec());
		if (testPuzzle->isCompleted())
		{
			player->setPos(vec3(0.0f, 0.0f, 0.0f));
			generationManager->initialize();
		}
	}
}

void Game::HandlePlayer()
{
	if (player->getPos().y < maxDepth) {
		player->TakeDmg();
		player->Reset();
		ghost->Reset();
	}
}
