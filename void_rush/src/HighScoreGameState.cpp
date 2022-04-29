#include "HighScoreGameState.h"

HighScoreGameState::HighScoreGameState(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam):
	GameState(gfx,rm,imguimanager,mouse,keyboard,cam)
{
	readHighScoreFile();
	UI = new UIManager(rm, gfx);
	UI->createUIButton("assets/textures/outline.png", "back", mouse, vec2(-1, 0.8), vec2(0.2, 0.2), "back", vec2(0,0.1));
	for (int i = 0; i < 5; i++) {
		UI->createUIString(scores[i], vec2(-0.75, 0.5 - i * 0.2),vec2(0.1,0.1));
	}
	std::string skyboxTextures[6] = {
	"assets/textures/Skybox/posx.png",//x+
	"assets/textures/Skybox/negx.png",//x-
	"assets/textures/Skybox/posy.png",//y+
	"assets/textures/Skybox/negy.png",//y-
	"assets/textures/Skybox/posz.png",//z+
	"assets/textures/Skybox/negz.png"//z-
	};
	skybox = new SkyBox(rm->get_Models("skybox_cube.obj", gfx), gfx, vec3(0, 0, 0), rm->getSpriteCube(skyboxTextures,gfx));
}

HighScoreGameState::~HighScoreGameState()
{
	delete UI;
	delete skybox;
}

void HighScoreGameState::handleEvents()
{
}

void HighScoreGameState::renderShadow()
{
	//Nothing in here
}

GameStatesEnum HighScoreGameState::update(float dt)
{
	GameStatesEnum theReturn = GameStatesEnum::NO_CHANGE;
	UI->update();
	if (UI->getButton("back")->clicked()) {
		theReturn = GameStatesEnum::TO_MENU;
	}
	camera->addRotation(vec3(dt * 0.001, dt * 0.01, 0));
	return theReturn;
}

void HighScoreGameState::render()
{
	gfx->setRenderTarget();
	gfx->setTransparant(true);

	gfx->get_IMctx()->OMSetRenderTargets(1, &gfx->getRenderTarget(), nullptr);
	skybox->draw(gfx);
	UI->draw();
	gfx->get_IMctx()->OMSetRenderTargets(1, &gfx->getRenderTarget(), gfx->getDepthStencil());

	gfx->present();
}

void HighScoreGameState::readHighScoreFile()
{
	std::string line;
	int i, numberOfScores;
	i = 0;
	numberOfScores = 0;
	std::ifstream highscoreFile("assets/files/highScores.txt");
	if (highscoreFile.is_open()) {
		if (std::getline(highscoreFile, line)) {
			//we assume first is a int
			numberOfScores = std::atoi(line.c_str());
		}
		while (std::getline(highscoreFile, line) && i < numberOfScores)
		{
			//need seperate these later
			scores[i] = line;
			i++;
		}
		highscoreFile.close();
	}
	else {
		std::cout << "couldn't open highscore file" << std::endl;
	}

}
