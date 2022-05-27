#include "powerUpManager.hpp"

PowerupManager::PowerupManager(GameObjectManager* GameObjManager, Graphics* gfx, ResourceManager* rm, CollisionHandler* collisionHandler, Mouse* mouse, Keyboard* keyboard )
	: GameObjManager(GameObjManager), gfx(gfx), rm(rm), collisionHandler(collisionHandler), keyboard(keyboard), mouse(mouse),
	player(nullptr), ghost(nullptr)
{	
}

void PowerupManager::init(Player* player, Ghost* ghost)
{
	this->player = player; 
	this->ghost = ghost;

	///////POWERUPS///////
	create_GoldenApple();
	create_Potion();
	create_Feather();
	create_Shield();
	create_Money();
	create_Snowflake();
	create_Pearl();
	create_Rocket();

}

void PowerupManager::update()
{
	static float amount = 0.15f;
	static float randomOffset = 0.f;
	static float speed = 50;
	static DeltaTime timer;	
	static double time = 0.0;	
	static float dt;
	timer.restartClock();
	dt = timer.dt();
	time += timer.dt();

	for (int i = 0; i < this->powers.size(); i++) { 
		randomOffset = 0.1f / (rand() % 10 + 1);
		
		if (!this->powers[i]->pickedUp) {

			this->powers[i]->addRot(vec3(0.f, deg_to_rad(1.f+ randomOffset) * speed * dt, 0.f));	
			this->powers[i]->setPos(this->powers[i]->recievedPos + vec3(0.f, sinf(time + dt) * amount, 0.f));			
		}
				
	}

}

void PowerupManager::reset()
{
	nrOfActive = nrOfPowerups();

	for (auto& p : this->powers ) {
		p->setPos(vec3(1000.0f, 1000.0f, 1000.0f));
		p->recievedPos = vec3(1000.0f, 1000.0f, 1000.0f);
		p->setPickedUp(false);
	}
}

GameObject*& PowerupManager::get_GoldenApple()
{
	int index = this->nrOfActive.GoldenApples;
	if (this->nrOfActive.GoldenApples >= this->nrOf.GoldenApples ) {
		create_GoldenApple();
	}
	else {
		this->nrOfActive.GoldenApples++;
	}	
	return this->GameObjManager->getGameObject("Apple"+std::to_string(index));		
}

GameObject*& PowerupManager::get_Feather()
{
	int index = this->nrOfActive.Feathers;
	if (this->nrOfActive.Feathers >= this->nrOf.Feathers ) {
		create_Feather();
	}
	else {
		this->nrOfActive.Feathers++;
	}	
	return this->GameObjManager->getGameObject("Feather"+std::to_string(index));
}

GameObject*& PowerupManager::get_Potion()
{
	int index = this->nrOfActive.Potions;
	if (this->nrOfActive.Potions >= this->nrOf.Potions ) {
		create_Potion();
	}
	else {
		this->nrOfActive.Potions++;
	}	
	return this->GameObjManager->getGameObject("Potion"+std::to_string(index));
}

GameObject*& PowerupManager::get_Shield()
{
	int index = this->nrOfActive.Shields;
	if (this->nrOfActive.Shields >= this->nrOf.Shields ) {
		create_Shield();
	}
	else {
		this->nrOfActive.Shields++;
	}	
	return this->GameObjManager->getGameObject("Shield"+std::to_string(index));
}

GameObject*& PowerupManager::get_Money()
{
	int index = this->nrOfActive.Money;
	if (this->nrOfActive.Money >= this->nrOf.Money ) {
		create_Money();
	}
	else {
		this->nrOfActive.Money++;
	}	
	return this->GameObjManager->getGameObject("Money"+std::to_string(index));
}

GameObject*& PowerupManager::get_Snowflake()
{
	int index = this->nrOfActive.Snowflakes;
	if (this->nrOfActive.Snowflakes >= this->nrOf.Snowflakes ) {
		create_Snowflake();
	}
	else {
		this->nrOfActive.Snowflakes++;
	}	
	return this->GameObjManager->getGameObject("Snowflake"+std::to_string(index));
}

GameObject*& PowerupManager::get_Pearl()
{
	int index = this->nrOfActive.Pearls;
	if (this->nrOfActive.Pearls >= this->nrOf.Pearls ) {
		create_Pearl();
	}
	else {
		this->nrOfActive.Pearls++;
	}	
	return this->GameObjManager->getGameObject("Pearl"+std::to_string(index));
}

GameObject*& PowerupManager::get_Rocket()
{
	int index = this->nrOfActive.Rockets;
	if (this->nrOfActive.Rockets >= this->nrOf.Rockets ) {
		create_Rocket();
	}
	else {
		this->nrOfActive.Rockets++;
	}	
	return this->GameObjManager->getGameObject("Rocket"+std::to_string(index));
}

void PowerupManager::create_GoldenApple()
{
	std::string name = "Apple" + std::to_string(nrOf.GoldenApples++);
	nrOfActive.GoldenApples = nrOf.GoldenApples;
	powers.push_back(new Powerups(rm->get_Models("GoldenApple.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000.0f, 1000.0f, 1000.0f),
			 vec3(0.0f, 0.0f, 0.0f),
			 vec3(0.1f, 0.1f, 0.1f),
			 APPLE));
	
	GameObjManager->addGameObject(powers.back(), name);
	collisionHandler->addPowerups(powers.back());
}

void PowerupManager::create_Feather()
{
	std::string name = "Feather" + std::to_string(nrOf.Feathers++);
	nrOfActive.Feathers = nrOf.Feathers;
	powers.push_back(new Powerups(rm->get_Models("Feather.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000.0f, 1000.0f, 1000.0f),
			 vec3(30.0f, 40.0f, 30.0f),
			 vec3(0.17f, 0.17f, 0.17f),
			 FEATHER));
	
	GameObjManager->addGameObject(powers.back(), name);
	collisionHandler->addPowerups(powers.back());
}

void PowerupManager::create_Potion()
{
	std::string name = "Potion" + std::to_string(nrOf.Potions++);
	nrOfActive.Potions = nrOf.Potions;
	powers.push_back(new Powerups(rm->get_Models("Potion.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000.0f, 1000.0f, 1000.0f),
			 vec3(0.0f, deg_to_rad(30.f), deg_to_rad(10.f)),
			 vec3(0.2f, 0.2f, 0.2f),
			 POTION));
	
	GameObjManager->addGameObject(powers.back(), name);
	collisionHandler->addPowerups(powers.back());
}

void PowerupManager::create_Shield()
{
	std::string name = "Shield" + std::to_string(nrOf.Shields++);
	nrOfActive.Shields = nrOf.Shields;
	powers.push_back(new Powerups(rm->get_Models("Shield.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000, 1000, 1000),
			 vec3(0, 0, 0),
			 vec3(0.25, 0.25, 0.25),
			 SHIELD));
	
	GameObjManager->addGameObject(powers.back(), name);
	collisionHandler->addPowerups(powers.back());
}

void PowerupManager::create_Money()
{
	std::string name = "Money" + std::to_string(nrOf.Money++);
	nrOfActive.Money = nrOf.Money;
	powers.push_back(new Powerups(rm->get_Models("Money.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000.0f, 1000.0f, 1000.0f),
			 vec3(0.0f, 90.0f, 90.0f),
			 vec3(0.1f, 0.1f, 0.1f),
			 MONEY));
	
	GameObjManager->addGameObject(powers.back(), name);
	collisionHandler->addPowerups(powers.back());
}

void PowerupManager::create_Snowflake()
{
	std::string name = "Snowflake" + std::to_string(nrOf.Snowflakes++);
	nrOfActive.Snowflakes = nrOf.Snowflakes;
	powers.push_back(new Powerups(rm->get_Models("Snowflake.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000.0f, 1000.0f, 1000.0f),
			 vec3(0.0f, 0.0f, 90.0f),
			 vec3(0.3f, 0.3f, 0.3f),
			 FREEZE)); {
		
	GameObjManager->addGameObject(powers.back(), name);
		collisionHandler->addPowerups(powers.back());
	}
}
void PowerupManager::create_Pearl()
{
	std::string name = "Pearl" + std::to_string(nrOf.Pearls++);
	nrOfActive.Pearls = nrOf.Pearls;
	powers.push_back(new Powerups(rm->get_Models("Pearl.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000.0f, 1000.0f, 1000.0f),
			 vec3(0.0f, 0.0f, 0.0f),
			 vec3(0.075f, 0.075f, 0.075f),
			 PEARL));
	
	GameObjManager->addGameObject(powers.back(), name);
	collisionHandler->addPowerups(powers.back());
}

void PowerupManager::create_Rocket()
{
	std::string name = "Rocket" + std::to_string(nrOf.Rockets++);
	nrOfActive.Rockets = nrOf.Rockets;
	powers.push_back(new Powerups(rm->get_Models("Rocket.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000.0f, 1000.0f, 1000.0f),
			 vec3(0.0f, deg_to_rad(30.f), deg_to_rad(-45.f)),
			 vec3(0.085f, 0.085f, 0.085f),
			 ROCKET));
	
	GameObjManager->addGameObject(powers.back(), name);
	collisionHandler->addPowerups(powers.back());
}

void PowerupManager::setUpPowerups(int chosenDiff, vec3 pos)
{
	GameObject* powerUpObject = nullptr;
	int chosenPower = 1 + (rand() % 100);
	//Difficulty easy
	if (chosenDiff == 1)
	{
		if (chosenPower <= 48)
		{
			//Got F-tier
			if (chosenPower <= 12)
			{
				//choose feather and moved into position
				powerUpObject = get_Feather();
				powerUpObject->setPos(pos);
				powerUpObject->setWannaDraw(true);

			}
			else if (chosenPower > 12 && chosenPower <= 24)
			{
				//choose speed and moved into position
				powerUpObject = get_Potion();
				powerUpObject->setPos(pos);
				powerUpObject->setWannaDraw(true);

			}
			else if (chosenPower > 24 && chosenPower <= 36)
			{
				//choose shield and moved into position
				powerUpObject = get_Shield();
				powerUpObject->setPos(pos);
				powerUpObject->setWannaDraw(true);
			}
			else if (chosenPower > 36 && chosenPower <= 48)
			{
				// choose Money and moved into position
				powerUpObject = get_Money();
				powerUpObject->setPos(pos);
				powerUpObject->setWannaDraw(true);
			}
		}
		else if (chosenPower > 48 && chosenPower <= 84)
		{
			//Got C-tier
			if (chosenPower > 48 && chosenPower <= 66)
			{
				//Freeze
				powerUpObject = get_Snowflake();
				powerUpObject->setPos(pos);
				powerUpObject->setWannaDraw(true);
			}
			else if (chosenPower > 66 && chosenPower <= 84)
			{
				//Pearl
				powerUpObject = get_Pearl();
				powerUpObject->setPos(pos);
				powerUpObject->setWannaDraw(true);
			}
		}
		else if (chosenPower > 84)
		{
			//Got S-tier
			if (chosenPower > 84 && chosenPower <= 92)
			{
				//Apple
				powerUpObject = get_GoldenApple();
				powerUpObject->setPos(pos);
				powerUpObject->setWannaDraw(true);
			}
			else if (chosenPower > 92 && chosenPower <= 100)
			{
				// Rocket
				powerUpObject = get_Rocket();
				powerUpObject->setPos(pos);
				powerUpObject->setWannaDraw(true);
			}
		}
	}
	if (powerUpObject) {
		(dynamic_cast<Powerups*>(powerUpObject))->recievedPos = pos;
	}
}
