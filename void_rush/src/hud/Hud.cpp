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
	UI->createUIString("00m", vec2(-0.27f, 0.65f), vec2(0.03f, 0.03f), "GhostDistance");
	UI->createUISprite("assets/textures/Zelda Heart.png", vec2(0.8f, 0.88f), vec2(0.1f, 0.1f)); //Change to score and add text that updates.
	UI->createUISprite("assets/textures/PowerUpFrame.png", vec2(0.7f, -0.95f), vec2(0.3f, 0.3f));
	UI->createUISprite("assets/textures/Empty.png", vec2(0.75f, -0.63f), vec2(0.05f, 0.05f));
	UI->createUISprite("assets/textures/Empty.png", vec2(0.8f, -0.63f), vec2(0.05f, 0.05f));
	UI->createUISprite("assets/textures/Empty.png", vec2(0.85f, -0.63f), vec2(0.05f, 0.05f));
	UI->createUISprite("assets/textures/Empty.png", vec2(0.9f, -0.63f), vec2(0.05f, 0.05f));
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

void Hud::TurnOnPassive(int index)
{
	switch (index)
	{
	case 1:
		UI->getElements(PASSIVE1)->replaceSprite(rm->getSprite("assets/textures/Feather.png", gfx));
		passive1On = true;
		break;
	case 2:
		UI->getElements(PASSIVE2)->replaceSprite(rm->getSprite("assets/textures/Pearl.png", gfx));
		passive2On = true;
		break;
	case 3:
		UI->getElements(PASSIVE3)->replaceSprite(rm->getSprite("assets/textures/Potion.png", gfx));
		passive3On = true;
		break;
	case 4:
		UI->getElements(PASSIVE4)->replaceSprite(rm->getSprite("assets/textures/Shield.png", gfx));
		passive4On = true;
		break;
	}
}

void Hud::TurnOffPassive(int index)
{
	switch (index)
	{
	case 1:
		UI->getElements(PASSIVE1)->replaceSprite(rm->getSprite("assets/textures/Empty.png", gfx));
		passive1On = false;
		break;
	case 2:
		UI->getElements(PASSIVE2)->replaceSprite(rm->getSprite("assets/textures/Empty.png", gfx));
		passive2On = false;
		break;
	case 3:
		UI->getElements(PASSIVE3)->replaceSprite(rm->getSprite("assets/textures/Empty.png", gfx));
		passive3On = false;
		break;
	case 4:
		UI->getElements(PASSIVE4)->replaceSprite(rm->getSprite("assets/textures/Empty.png", gfx));
		passive4On = false;
		break;
	}
}

void Hud::ChangeCurrentPowerUp(int index)
{
	switch (index)
	{
	case 0:
		UI->getElements(POWERUP)->replaceSprite(rm->getSprite("assets/textures/PowerUpFrame.png", gfx));
		currentPowerUp = EMPTY;
		break;
	case 1:
		UI->getElements(POWERUP)->replaceSprite(rm->getSprite("assets/textures/PowerUpRocket.png", gfx));
		currentPowerUp = ROCKET;
		break;
	case 2:
		UI->getElements(POWERUP)->replaceSprite(rm->getSprite("assets/textures/PowerUpCard.png", gfx));
		currentPowerUp = CARD;
		break;
	case 3:
		UI->getElements(POWERUP)->replaceSprite(rm->getSprite("assets/textures/PowerUpFreeze.png", gfx));
		currentPowerUp = FREEZE;
		break;
	case 4:
		UI->getElements(POWERUP)->replaceSprite(rm->getSprite("assets/textures/PowerUpDeath.png", gfx));
		currentPowerUp = DEATH;
		break;
	case 5:
		UI->getElements(POWERUP)->replaceSprite(rm->getSprite("assets/textures/PowerUpEMP.png", gfx));
		currentPowerUp = EMP;
		break;
	case 6:
		UI->getElements(POWERUP)->replaceSprite(rm->getSprite("assets/textures/PowerUpPad.png", gfx));
		currentPowerUp = PAD;
		break;
	}
}

bool Hud::GetStatusOfPassive(int index)
{
	switch (index)
	{
	case 1:
		return passive1On;
		break;
	case 2:
		return passive2On;
		break;
	case 3:
		return passive3On;
		break;
	case 4:
		return passive4On;
		break;
	}

	return false;
}

void Hud::UpdateGhostBar(vec3 playerPos, vec3 puzzlePos, vec3 ghostPos, float totalDistance)
{
	float currentDistance = (puzzlePos - playerPos).length();

	float currentGhostDistance = (ghostPos - playerPos).length();

	sizeOfBar = (1.0f - (currentDistance / totalDistance)) * this->fullBar;

	if (sizeOfBar > 0.4f)
	{
		sizeOfBar = this->fullBar;
		UI->getElements(GHOSTBAR)->setSize(sizeOfBar, 0.1f);
	}
	else
	{
		sizeOfBar = sizeOfBar + ((sizeOfBar / 0.4f) * 0.067f);
		UI->getElements(GHOSTBAR)->setSize(sizeOfBar, 0.1f);
	}

	std::string meterText;
	if (currentGhostDistance < 10.0f)
	{
		meterText = "0" + std::to_string((int)currentGhostDistance) + "m";
	}
	else if (currentGhostDistance < 100.0f)
	{
		meterText = std::to_string((int)currentGhostDistance) + "m";
	}
	else
	{
		meterText = "99m";
	}
	UI->getStringElement("GhostDistance")->setText(meterText);
	
	currentGhostDistance = currentGhostDistance / 100.0f;
	if (currentGhostDistance > 1.0f)
	{
		currentGhostDistance = 1.0f;
	}
	
	if (sizeOfBar <= 0.0f)
	{
		sizeOfBar = 0.001f;
	}

	sizeOfBar = (1.0f - currentGhostDistance) * sizeOfBar;

	UI->getElements(GHOSTICON)->setPosition((-0.26f + sizeOfBar), 0.71f);
	UI->getStringElement("GhostDistance")->setPosition(vec2(-0.27f + sizeOfBar, 0.65f));
}

void Hud::Update()
{
	UI->draw();
}
