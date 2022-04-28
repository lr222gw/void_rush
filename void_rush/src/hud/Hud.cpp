#include "Hud.h"

Hud::Hud(Graphics*& gfx, ResourceManager*& rm) : gfx(gfx), rm(rm)
{
	setUpUI();
}

Hud::~Hud()
{
	delete UI;
}

void Hud::setUpUI()
{
	UI = new UIManager(rm, gfx);
	UI->createUIString("x3", vec2(-0.88f, 0.9f), vec2(0.05f, 0.05f), "Health");
	UI->createUISprite("assets/textures/Zelda Heart.png", vec2(-0.99f, 0.88f), vec2(0.1f, 0.1f));
	UI->createUISprite("assets/textures/Ghost Frame.png", vec2(-0.25f, 0.8f), vec2(0.5f, 0.2f));
}

void Hud::ResetHUD()
{
	currentHealth = 3;
	std::string health = "x" + std::to_string(currentHealth);
	UI->getStringElement("Health")->setText(health);
}

void Hud::LowerHealth()
{
	currentHealth--;
	std::string health = "x" + std::to_string(currentHealth);
	UI->getStringElement("Health")->setText(health);
}

void Hud::IncreaseHealth()
{
	currentHealth++;
	std::string health = "x" + std::to_string(currentHealth);
	UI->getStringElement("Health")->setText(health);
}

void Hud::Update()
{
	UI->draw();
}
