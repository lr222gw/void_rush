#include "GameState.h"

GameState::GameState(Graphics*& gfx, ResourceManager*& rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam)
{
	this->gfx = gfx;
	this->rm = rm;
	this->IMGUI = imguimanager;
	this->camera = cam;
	this->keyboard = keyboard;
	this->mouse = mouse;
}

GameState::~GameState()
{
}

void GameState::handleEvents()
{
}

void GameState::renderShadow()
{
}

GameStateRet GameState::update(float dt)
{
	GameStateRet ret{GameStatesEnum(), 1};
	return ret;
}

void GameState::render()
{
}
