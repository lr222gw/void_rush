#include "SettingsScene.h"

SettingsScene::SettingsScene(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam):
	GameState(gfx,rm,imguimanager,mouse,keyboard,cam)
{
	UI = new UIManager(rm, gfx);

	readSettings();

	UI->createUIButton("assets/textures/buttonBack.png", "Back", mouse, vec2(-1, 0.8f), vec2(0.2f, 0.2f), "Back");


	UI->createUIString("Volume", vec2(-1.0f, 0.51f), vec2(0.06f, 0.1f), "Volume");
	UI->createUIString("00", vec2(-0.1f, 0.51f), vec2(0.1f, 0.1f), "VolumeFloat");
	UI->getStringElement("VolumeFloat")->setText(VolumeToString());
	UI->createUIButton("assets/textures/buttonBack.png", "-", mouse, vec2(-0.3f, 0.5f), vec2(0.1f, 0.1f), "LVol");
	UI->createUIButton("assets/textures/buttonBack.png", "+", mouse, vec2(0.5f, 0.5f), vec2(0.1f, 0.1f), "HVol");

	UI->createUIString("Mouse", vec2(-1.0f, 0.251f), vec2(0.06f, 0.1f), "Mouse");
	UI->createUIString("0.0", vec2(-0.1f, 0.25f), vec2(0.1f, 0.1f), "SenceFloat");
	UI->getStringElement("SenceFloat")->setText(std::to_string(sett.mouseSence));
	UI->createUIButton("assets/textures/buttonBack.png", "-", mouse, vec2(-0.3f, 0.25f), vec2(0.1f, 0.1f), "LSence");
	UI->createUIButton("assets/textures/buttonBack.png", "+", mouse, vec2(0.5f, 0.25f), vec2(0.1f, 0.1f), "HSence");

	UI->createUIString("Resolution", vec2(-1.0f, 0.01f), vec2(0.06f, 0.1f), "ResolutionString");
	UI->createUIString("0000x0000", vec2(-0.1f, 0.01f), vec2(0.06f, 0.1f), "Resolution");
	UI->getStringElement("Resolution")->setText(ResolutionToString());
	UI->createUIButton("assets/textures/buttonBack.png", "<", mouse, vec2(-0.3f, 0.0f), vec2(0.1f, 0.1f), "LRes");
	UI->createUIButton("assets/textures/buttonBack.png", ">", mouse, vec2(0.5f, 0.0f), vec2(0.1f, 0.1f), "HRes");

	UI->createUIString("FullScreen", vec2(-1.0f, -0.25f), vec2(0.06f, 0.1f), "FullScreen");
	UI->createUIButton("assets/textures/buttonBack.png", "Y", mouse, vec2(-0.3f, -0.25f), vec2(0.15f, 0.15f), "FullScreenButton");
	UI->getStringElement("FullScreenButton")->setText(FullScreenToString());

	UI->createUIButton("assets/textures/buttonBack.png", "Save", mouse, vec2(-0.9f, -0.75f), vec2(0.25f, 0.2f), "Save");

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
	gfx->takeLight(nullptr, 0);
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
	else if (UI->getButton("HVol")->clicked()) {
		++sett.volume;
		UI->getStringElement("VolumeFloat")->setText(VolumeToString());
	}
	else if (UI->getButton("LVol")->clicked()) {
		--sett.volume;
		UI->getStringElement("VolumeFloat")->setText(VolumeToString());
	}
	else if (UI->getButton("HSence")->clicked()) {
		sett.mouseSence += 0.1f;
		UI->getStringElement("SenceFloat")->setText(std::to_string(sett.mouseSence));
	}
	else if (UI->getButton("LSence")->clicked()) {
		sett.mouseSence -= 0.1f;
		if (sett.mouseSence < 0) {
			sett.mouseSence = 0;
		}
		UI->getStringElement("SenceFloat")->setText(std::to_string(sett.mouseSence));
	}
	else if (UI->getButton("HRes")->clicked()) {
		++sett.resolution;
		UI->getStringElement("Resolution")->setText(ResolutionToString());
	}
	else if (UI->getButton("LRes")->clicked()) {
		--sett.resolution;
		UI->getStringElement("Resolution")->setText(ResolutionToString());
	}
	else if (UI->getButton("FullScreenButton")->clicked()) {
		sett.fullscreen = !sett.fullscreen;
		UI->getStringElement("FullScreenButton")->setText(FullScreenToString());
	}
	else if (UI->getButton("Save")->clicked()) {
		saveSettings();
	}
	if (savedTime > 0 && exist) {
		savedTime -= dt;
	}
	if (savedTime < 0 && exist) {
		UI->deleteString("saved");
		exist = false;
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
	std::ifstream input("Setting.data", std::ios::out | std::ios::binary);
	if (!input) {
		sett.volume = 1;
		sett.resolution = 4;
		sett.mouseSence = 0.6f;
	}
	input.read((char*)&sett, sizeof(sett));
	input.close();
	settingsSingleTon::GetInst().setSettings(sett);
}

void SettingsScene::saveSettings()
{
	UI->deleteString("saved");
	UI->createUIString("saved", vec2(-0.25f, -0.7f), vec2(0.1f, 0.1f), "saved");
	exist = true;
	savedTime = 2.f;
	std::ofstream output("Setting.data", std::ios::out | std::ios::binary);
	output.write((char*)&sett, sizeof(sett));
	output.close();
	settingsSingleTon::GetInst().setSettings(sett);
}

std::string SettingsScene::VolumeToString()
{
	if (sett.volume < 0) {
		sett.volume = 0;
	}
	else if (sett.volume > 99) {
		sett.volume = 99;
	}
	if (sett.volume < 10) {
		return " " + std::to_string(sett.volume);
	}
	else {
		return std::to_string(sett.volume);
	}
}

std::string SettingsScene::ResolutionToString()
{
	if (sett.resolution > 4) {
		sett.resolution = 4;
	}
	else if (sett.resolution < 0) {
		sett.resolution = 0;
	}
	std::string width;
	std::string height;
	width = std::to_string(resolutions[sett.resolution][0]);
	if (width.size() < 4) {
		width = ' ' + width;
	}
	height = std::to_string(resolutions[sett.resolution][1]);
	if (height.size() < 4) {
		height += ' ';
	}
	return width + "x" + height;
}

std::string SettingsScene::FullScreenToString()
{
	if (sett.fullscreen) {
		return "Y";
	}
	else {
		return "N";
	}
}
