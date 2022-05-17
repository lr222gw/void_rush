#include "SettingsScene.h"

SettingsScene::SettingsScene(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam):
	GameState(gfx,rm,imguimanager,mouse,keyboard,cam)
{
	UI = new UIManager(rm, gfx);

	UI->createUIButton("buttonBack.png", "Back", mouse, vec2(-1, 0), vec2(0.2, 0.2), "Back");
	UI->createUIString("0", vec2(1, 0), vec2(0.2, 0.2), "VolumeFloat");
	UI->createUIButton("buttonBack.png", "+", mouse, vec2(-0.5, 0), vec2(0.2, 0.2), "HVol");
	UI->createUIButton("buttonBack.png", "-", mouse, vec2(0.5, 0), vec2(0.2, 0.2), "LVol");

	readSettings();

	std::string skyboxTextures[6] = {
		"assets/textures/Skybox/posx.png",//x+
		"assets/textures/Skybox/negx.png",//x-
		"assets/textures/Skybox/posy.png",//y+
		"assets/textures/Skybox/negy.png",//y-
		"assets/textures/Skybox/posz.png",//z+
		"assets/textures/Skybox/negz.png"//z-
	};
	skybox = new SkyBox(rm->get_Models("skybox_cube.obj", gfx), gfx, vec3(0, 0, 0), rm->getSpriteCube(skyboxTextures, gfx));

}

SettingsScene::~SettingsScene()
{
	delete UI;
	delete skybox;
}

void SettingsScene::handleEvents()
{
}

void SettingsScene::renderShadow()
{
}

GameStateRet SettingsScene::update(float dt)
{
	GameStateRet theReturn;
	theReturn.gameState = GameStatesEnum::NO_CHANGE;

	UI->update();

	if (UI->getButton("Back")->clicked()) {
		theReturn.gameState = GameStatesEnum::TO_MENU;
	}
	if (UI->getButton("HVol")->clicked()) {
		//add more to volume
	}
	if (UI->getButton("LVol")->clicked()) {
		//volume--;
	}

	camera->updateCamera();
	camera->addRotation(vec3(0.1f * dt, 0.3f * dt, 0));
	return theReturn;
}

void SettingsScene::render()
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

	gfx->present();
}

void SettingsScene::readSettings()
{
}

void SettingsScene::saveSettings()
{
}
