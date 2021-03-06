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
	vec2 wh = gfx->getClientWH();
	float passive_width = 0.05f;
	float passive_width_heart = 0.065f;
	float passive_height = wh.x / wh.y * passive_width;
	float passive_height_heart = wh.x / wh.y * passive_width_heart;

	UI = new UIManager(rm, gfx);
	UI->createUIString("x3", vec2(-0.88f, 0.87f), vec2(0.05f, 0.05f), "Health");
	UI->createUISprite("assets/textures/apple_heart.png", vec2(-0.94f, 0.86f), vec2(passive_width_heart, passive_height_heart));
	UI->createUISprite("assets/textures/Ghost Bar.png", vec2(-0.233f, 0.85f), vec2(this->noBar, 0.1f));
	UI->createUISprite("assets/textures/Ghost Frame.png", vec2(-0.25f, 0.8f), vec2(0.5f, 0.2f));
	UI->createUISprite("assets/textures/Ghost Pointer.png", vec2(-0.26f, 0.71f), vec2(0.05f, 0.1f));
	UI->createUIString("00m", vec2(-0.23f, 0.65f), vec2(0.045f, 0.045f), "GhostDistance");
	UI->createUISprite("assets/textures/ScoreText.png", vec2(0.54f, 0.88f), vec2(0.15f, 0.1f));
	UI->createUIString("000000", vec2(0.7f, 0.88f), vec2(0.05f, 0.08f), "Score");
	UI->createUISprite("assets/textures/PowerUpFrame.png", vec2(0.7f, -0.95f), vec2(0.3f, 0.3f));
	UI->createUISprite("assets/textures/Empty.png", vec2(0.75f, -0.63f),	vec2(passive_width,passive_height));
	UI->createUISprite("assets/textures/Empty.png", vec2(0.8f, -0.63f),		vec2(passive_width,passive_height));
	UI->createUISprite("assets/textures/Empty.png", vec2(0.85f, -0.63f),	vec2(passive_width,passive_height));
	UI->createUISprite("assets/textures/Empty.png", vec2(0.9f, -0.63f),		vec2(passive_width,passive_height));
	UI->createUISprite("assets/textures/Crosshair.png", vec2(-0.01f, -0.015f), vec2(0.02f, 0.03f));
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

void Hud::UpdateScore(int points)
{
	this->score = points;
	std::string scoreString;
	if (this->score <= 9)
	{
		scoreString = "00000" + std::to_string(this->score);
		UI->getStringElement("Score")->setText(scoreString);
	}
	else if (this->score <= 99)
	{
		scoreString = "0000" + std::to_string(this->score);
		UI->getStringElement("Score")->setText(scoreString);
	}
	else if (this->score <= 999)
	{
		scoreString = "000" + std::to_string(this->score);
		UI->getStringElement("Score")->setText(scoreString);
	}
	else if (this->score <= 9999)
	{
		scoreString = "00" + std::to_string(this->score);
		UI->getStringElement("Score")->setText(scoreString);
	}
	else if (this->score <= 99999)
	{
		scoreString = "0" + std::to_string(this->score);
		UI->getStringElement("Score")->setText(scoreString);
	}
	else if (this->score <= 999999)
	{
		scoreString = std::to_string(this->score);
		UI->getStringElement("Score")->setText(scoreString);
	}
	else
	{
		UI->getStringElement("Score")->setText("999999");
	}
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
		UI->getElements(POWERUP)->replaceSprite(rm->getSprite("assets/textures/PowerUpFreeze.png", gfx));
		currentPowerUp = FREEZE;
		break;
	}
}

bool Hud::GetStatusOfPassive(int index) const
{
	switch (index)
	{
	case 1:
		return passive1On; //feater
		break;
	case 2:
		return passive2On; //pearl
		break;
	case 3:
		return passive3On; //potion
		break;
	case 4:
		return passive4On; //shield
		break;
	}

	return false;
}

int Hud::GetCurrentPowerUp() const
{
	return this->currentPowerUp;
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
