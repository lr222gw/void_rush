#include "Scene.h"

GameState::GameState(Graphics*& gfx, ResourceManager* rm, ImguiManager* imguimanager, Mouse* mouse, Keyboard* keyboard)
{
	this->gfx = gfx;
	this->rm = rm;
	this->IMGUI = imguimanager;
}
