#include "HighScoreGameState.h"

HighScoreGameState::HighScoreGameState(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam):
	GameState(gfx,rm,imguimanager,mouse,keyboard,cam)
{
	//Check if folder exists
	char* t;

	auto size = sizeof(t);
	_dupenv_s(&t, &size, "APPDATA");
	std::string path = std::string(t) + "\\void_rush";
	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directory(path);
	}
	path = path + "\\highscore.txt";
	if (!std::filesystem::exists(path)) {
		std::ofstream f(path);
		f.close();
	}
	this->pathToHighScore = path;
	free(t);
	readHighScoreFile();
	UI = new UIManager(rm, gfx);
	UI->createUIButton("assets/textures/buttonBack.png", "back", mouse, vec2(-1, 0.8f), vec2(0.2f, 0.2f), "back", vec2(0,0.1f));
	for (int i = 0; i < numberOfHighScore; i++) {
		UI->createUIString(scores[i], vec2(-0.7f, 0.88f - i * 0.2f),vec2(0.09f,0.1f));
		UI->createUIString(seeds[i], vec2(-0.7f, 0.83f - i * 0.2f), vec2(0.03f, 0.03f));
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

GameStateRet HighScoreGameState::update(float dt)
{
	GameStateRet theReturn;
	theReturn.gameState = GameStatesEnum::NO_CHANGE;
	theReturn.seed = 0;
	UI->update();
	if (UI->getButton("back")->clicked()) {
		theReturn.gameState = GameStatesEnum::TO_MENU;
	}
	camera->updateCamera();
	camera->addRotation(vec3(0.1f * dt, 0.3f * dt, 0));
	return theReturn;
}

void HighScoreGameState::render()
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

void HighScoreGameState::readHighScoreFile()
{
	std::string line;
	int i, numberOfScores;
	i = 0;
	numberOfScores = 0;
	//std::ifstream highscoreFile("assets/files/highScores.txt");
	std::ifstream highscoreFile(this->pathToHighScore);
	if (highscoreFile.is_open()) {
		if (std::getline(highscoreFile, line)) {
			//we assume first is a int
			numberOfScores = std::atoi(line.c_str());
		}
		if (numberOfScores > numberOfHighScore) {
			numberOfScores = numberOfHighScore;
		}
		while (std::getline(highscoreFile, line) && i < numberOfScores)
		{
			std::istringstream a;
			a.str(line);
			std::string name;
			std::string score;
			a >> name >> score >> seeds[i];
			//need seperate these later
			scores[i] = name + " " + score;
			i++;
		}
		highscoreFile.close();
	}
	else {
		std::cout << "couldn't open highscore file" << std::endl;
	}
	for (int i = 0; i < numberOfScores; i++) {
		std::replace(scores[i].begin(), scores[i].end(), '_', ' ');
	}

}
