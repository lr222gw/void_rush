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
	UI->createUISprite("assets/textures/Ghost Bar.png", vec2(-0.233f, 0.85f), vec2(this->noBar, 0.1f));
	UI->createUISprite("assets/textures/Ghost Frame.png", vec2(-0.25f, 0.8f), vec2(0.5f, 0.2f));
	UI->createUISprite("assets/textures/Ghost Pointer.png", vec2(-0.26f, 0.71f), vec2(0.05f, 0.1f));
	UI->createUIString("1000m", vec2(-0.35f, 0.6f), vec2(0.05f, 0.05f), "GhostDistance");
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

void Hud::UpdateGhostBar(vec3 playerPos, vec3 puzzlePos, vec3 ghostPos, float totalDistance)
{
	float currentDistance = (puzzlePos - playerPos).length();

	float currentGhostDistance = (ghostPos - playerPos).length();

	sizeOfBar = (1.0f - (currentDistance / totalDistance)) * this->fullBar;

	if (sizeOfBar > 0.4f)
	{
		UI->getElements(GHOSTBAR)->setSize(this->fullBar, 0.1f);
	}
	else
	{
		sizeOfBar = sizeOfBar + ((sizeOfBar / 0.4f) * 0.067f);
		UI->getElements(GHOSTBAR)->setSize(sizeOfBar, 0.1f);
	}

	std::string meterText;
	if (currentGhostDistance < 10.0f)
	{
		meterText = "000" + std::to_string((int)currentGhostDistance) + "m";
	}
	else if (currentGhostDistance < 100.0f)
	{
		meterText = "00" + std::to_string((int)currentGhostDistance) + "m";
	}
	else if (currentGhostDistance < 1000.0f)
	{
		meterText = "0" + std::to_string((int)currentGhostDistance) + "m";
	}
	else if (currentGhostDistance < 10000.0f)
	{
		meterText = std::to_string((int)currentGhostDistance) + "m";
	}
	else
	{
		meterText = "9999m";
	}
	UI->getStringElement("GhostDistance")->setText(meterText);
	
	currentGhostDistance = currentGhostDistance / 50.0f;
	if (currentGhostDistance > 1.0f)
	{
		currentGhostDistance = 1.0f;
	}
	
	if (sizeOfBar <= 0.0f)
	{
		sizeOfBar = 0.001f;
	}
	sizeOfBar = (1.0f - currentGhostDistance) * sizeOfBar;// * 0.467f;

	UI->getElements(GHOSTICON)->setPosition((-0.26f + sizeOfBar), 0.71f);
	UI->getStringElement("GhostDistance")->setPosition(vec2(-0.35f + sizeOfBar, 0.6f));
}

void Hud::Update()
{
	UI->draw();
}
