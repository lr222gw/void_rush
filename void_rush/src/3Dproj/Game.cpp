#include "Game.h"
#include <chrono>
#include <thread>
#include "Collision3D.h"

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
	
	setUpLights();
	
	//shadow map needs to take more lights
	this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx, 1920U, 1080U);
	//this->shadowMap = new ShadowMap((SpotLight**)light, nrOfLight, gfx, 640u, 360U);
	
	gfx->takeIM(&this->UIManager);
	
	camera = new Camera(gfx, mouse, vec3(0,0,0), vec3(0,0,0));
	camera->setData();
	//gfx->getWindosClass().setMouse(mouse);
	setUpObject();
	
	Qtree = new QuadTree(stataicObj, vec2(0, 0), 4, 100);
	//(pi,3.14) = 180 degrees
	Qtree->setUpCamProp(2000);	
	for (int i = 0; i < nrOfLight; i++) {
		LightVisualizers.push_back(new GameObject(rm->get_Models("Camera.obj"), gfx, light[i]->getPos(), light[i]->getRotation(), vec3(1.f, 1.0f, 1.0f)));
	}
	
	

	setUpParticles();
	UIManager.takeObject(obj[2]);
	UIManager.takeObject(obj[3]);
	/*IMGUI*/
	for (int i = 0; i < nrOfLight; i++) {
		UIManager.takeLight(light[i]);
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
		delete light[i];
		delete LightVisualizers[i];
	}
	delete[] light;
	for (int i = 0; i < obj.size(); i++) {
		delete obj[i];
	}
	for (int i = 0; i < stataicObj.size(); i++) {
		delete stataicObj[i];
	}
	delete Qtree;
	for (int i = 0; i < billboardGroups.size(); i++) {
		delete billboardGroups[i];
	}
	delete Space;
	delete player;
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

			soundManager.playSound("ah1", obj[0]->getPos());
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
}
printf("quit");
}

void Game::Update()
{
	/*Move things*/
	camera->updateCamera((float)dt.dt());
	if (getkey('N')) {
		DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-obj[1]->getPos().x, -obj[1]->getPos().y, -obj[1]->getPos().z, 1.0f
		);
		XRotation(viewMatrix, obj[1]->getRot().x);
		YRotation(viewMatrix, obj[1]->getRot().y);
		gfx->getVertexconstbuffer()->view.element = viewMatrix;
	}
	player->movePos(vec3(0, -12 * dt.dt(), 0));

	for (int i = 0; i < billboardGroups.size(); i++) {
		billboardGroups[i]->update((float)dt.dt(), gfx);
	}
	for (int i = 0; i < LightVisualizers.size(); i++) {
		LightVisualizers[i]->setPos(light[i]->getPos());
		LightVisualizers[i]->setRot(vec3(0 , light[i]->getRotation().x, -light[i]->getRotation().y) + vec3(0,1.57f,0));
	}
	camera->calcFURVectors();
	Space->update(camera->getPos());

	/*update matrixes*/
	for (int i = 0; i < obj.size(); i++) {
		obj[i]->updateMatrix();
	}
	player->updateMatrix();

	/*Collision checking*/
	collisionWithBlocking(player->getPlayerObjPointer(), obj[2]);
	collisionWithBlocking(player->getPlayerObjPointer(), obj[0]);
	/*if (collision3D(this->player->getBoundingBoxFromObject(), obj[2]->getBoundingBox())
		player->setGrounded();*/

	/*update vertex*/
	updateShaders();

	/*update things*/
	soundManager.update(camera->getPos(), camera->getForwardVec());
	gfx->Update((float)dt.dt(), camera->getPos());
	for (int i = 0; i < obj.size(); i++) {
		obj[i]->update();
	}
	player->update(dt.dt());

#pragma region camera_settings
	if (getkey('C')) {
		camera->setPosition(light[lightNr]->getPos());
		camera->setRotation(light[lightNr]->getRotation());
	}
	if (getkey('1')) {
		lightNr = 0;
	}
	if (getkey('2')) {
		lightNr = 1;
	}
	if (getkey('3')) {
		lightNr = 2;
	}
	if (getkey('4')) {
		lightNr = 3;
	} 
#pragma endregion camera_settings
}

void Game::DrawToBuffer()
{	
	

	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[0]);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);

	gfx->get_IMctx()->OMSetRenderTargets(1, &gfx->getRenderTarget(), nullptr);
	Space->draw(gfx);
	gfx->get_IMctx()->OMSetRenderTargets(1, &gfx->getRenderTarget(), gfx->getDepthStencil());

	gfx->get_IMctx()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	for (int i = 0; i < obj.size(); i++) {
		obj[i]->draw(gfx);
	}
	player->draw(gfx);

    camera->calcFURVectors();
	Qtree->draw(gfx, camera);
	Qtree->clearAlrDraw();
	
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[0], nullptr, 0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[0], nullptr, 0);
	
	
	if (getkey('F')) {
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->draw(gfx, false);
		}
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

void Game::DrawAllShadowObject()
{
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->PSSetShader(nullptr, nullptr, 0);
	for (int i = 0; i < obj.size(); i++) {
	
		obj[i]->draw(gfx, true);
	}
	camera->calcFURVectors();
	Qtree->draw(gfx, camera, true);
	Qtree->clearAlrDraw();
}

void Game::updateShaders(bool vs, bool ps)
{
	for (int i = 0; i < billboardGroups.size(); i++) {
		billboardGroups[0]->updateShader(gfx, camera->getPos());
	}
	
	if (vs)
	{
		for (int i = 0; i < obj.size(); i++) {
			obj[i]->updateVertexShader(gfx);
		}
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->updateVertexShader(gfx);
		}
		Space->updateVertexShader(gfx);
		player->updateVertexShader(gfx);
	}
	if (ps) {
		for (int i = 0; i < obj.size(); i++) {
			obj[i]->updatePixelShader(gfx);
		}
		for (int i = 0; i < LightVisualizers.size(); i++) {
			LightVisualizers[i]->updatePixelShader(gfx);
		}
		for (int i = 0; i < stataicObj.size(); i++) {
			stataicObj[i]->updatePixelShader(gfx);
		}
		Space->updatePixelShader(gfx);
		player->updatePixelShader(gfx);
	}
}

void Game::setUpObject()
{
	////////OBJECTS///////////
	//cameras
	obj.push_back(new GameObject(rm->get_Models("Camera.obj", gfx), gfx, vec3(0.f, 0.f, 10.f), vec3(0.f, 0.f, 0.f), vec3(5.f, 5.0f, 5.0f)));//main
	obj.push_back(new GameObject(rm->get_Models("Camera.obj", gfx), gfx, vec3(50.f, 0.f, 0.f), vec3(-1.58f, 0.f, 0.f), vec3(2.f, 2.0f, 2.0f)));//second
	////
	//////OBJECTS
	obj.push_back(new GameObject(rm->get_Models("quad2.obj", gfx), gfx, vec3(0, -5, 0), vec3(0, 0, 1.57f), vec3(100, 100, 100))); //Marken
	obj.push_back(new GameObject(rm->get_Models("nanosuit.obj", gfx), gfx, vec3(-5.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f)));

	obj.push_back(new GameObject(rm->get_Models("DCube.obj", gfx), gfx, vec3(5,0,0), vec3(0.f, 0.f, 0.f), vec3(5,5,5)));

	obj.push_back(new GameObject(rm->get_Models("DCube.obj", gfx), gfx, obj[0]->getPos(), vec3(0.f, 0.f, 0.f), vec3(0.5f, 0.5f, 0.5f)));
	obj.push_back(new GameObject(rm->get_Models("DCube.obj", gfx), gfx, obj[0]->getPos(), vec3(0.f, 0.f, 0.f), vec3(0.5f, 0.5f, 0.5f)));
	obj.push_back(new GameObject(rm->get_Models("DCube.obj", gfx), gfx, obj[0]->getPos(), vec3(0.f, 0.f, 0.f), vec3(0.5f, 0.5f, 0.5f)));
	obj.push_back(new GameObject(rm->get_Models("DCube.obj", gfx), gfx, obj[0]->getPos(), vec3(0.f, 0.f, 0.f), vec3(0.5f, 0.5f, 0.5f)));
	obj.push_back(new GameObject(rm->get_Models("fbxtest.fbx", gfx), gfx));

	//does not exists
	//obj.push_back(new GameObject(rm->get_Models("Shield.obj", gfx), gfx, vec3(-1,-1,-1), vec3(0.f, 0.f, 0.f), vec3(0.5f, 0.5f, 0.5f)));
	obj.push_back(new GameObject(rm->get_Models("quadYoda.obj", gfx), gfx, vec3(10,-1,-1), vec3(0.f, 0.f, 0.f), vec3(0.5f, 0.5f, 0.5f)));
	
	player = new Player(rm->get_Models("DCube.obj", gfx), gfx, camera, mouse, keyboard);
}

void Game::setUpLights()
{
	//current max number is set in graphics.cpp and transforms.hlsli
	nrOfLight = 1;
	light = new Light * [nrOfLight];

	//create the lights with 
	//light[0] = new DirLight(vec3(0, 30, 8), vec3(0.1f, -PI / 2, 1.f), 100, 100);
	light[0] = new PointLight(vec3(1, 1, 1), 200, vec3(1,1,1));
	//light[1] = new SpotLight(vec3(18, 46, 45), vec3(-2.4f, -0.5, 1));
	//light[2] = new SpotLight(vec3(8, 47.f, 0), vec3(0, -1, 1));
	//light[3] = new SpotLight(vec3(30, 50, 0), vec3(-1, -1, 1));

	//set color for lights (deafault white)
	light[0]->getColor() = vec3(1, 1, 1);
	//light[1]->getColor() = vec3(1, 0, 1);

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
