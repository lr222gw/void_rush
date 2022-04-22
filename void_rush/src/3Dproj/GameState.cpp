#include "GameState.h"

GameState::GameState(Graphics*& gfx, ResourceManager* rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard, Camera* cam)
{
	this->gfx = gfx;
	this->rm = rm;
	this->IMGUI = imguimanager;
	this->camera = cam;
}

void GameState::handleEvents()
{
}

void GameState::renderShadow()
{
}

GameStatesEnum GameState::update(float dt)
{
	return GameStatesEnum();
}

void GameState::render()
{
}
