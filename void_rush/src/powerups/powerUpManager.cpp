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
	create_EMP();
	create_Pad();
	create_Kill();
	create_Rocket();

}

void PowerupManager::update()
{
	static float invert = 1;
	float amount = 0.001f;
	float randomOffset = 0.f;
	static DeltaTime timer;	
	static double time = 0.0;	
	timer.restartClock();
	time += timer.dt();
	if(time > 1.0){
		time = 0.0;
		invert *= -1;
	}
	
	for (int i = 0; i < this->powers.size(); i++) { 
		randomOffset = 0.1f / (rand() % 10 + 1);
		this->powers[i]->addRot(vec3(0.f, deg_to_rad(1.f+ randomOffset), 0.f));
		this->powers[i]->movePos(vec3(0.f, amount*invert ,0.f));
	}
}

void PowerupManager::reset()
{
	nrOfActive = nrOfPowerups();

	for (auto& p : this->powers ) {
		p->setPos(vec3(1000.0f, 1000.0f, 1000.0f));
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

GameObject*& PowerupManager::get_EMP()
{
	int index = this->nrOfActive.EMPs;
	if (this->nrOfActive.EMPs >= this->nrOf.EMPs ) {
		create_EMP();
	}
	else {
		this->nrOfActive.EMPs++;
	}	
	return this->GameObjManager->getGameObject("EMP"+std::to_string(index));
}

GameObject*& PowerupManager::get_Pad()
{
	int index = this->nrOfActive.Pads;
	if (this->nrOfActive.Pads >= this->nrOf.Pads ) {
		create_Pad();
	}
	else {
		this->nrOfActive.Pads++;
	}	
	return this->GameObjManager->getGameObject("Pad"+std::to_string(index));
}

GameObject*& PowerupManager::get_Kill()
{
	int index = this->nrOfActive.Kills;
	if (this->nrOfActive.Kills >= this->nrOf.Kills ) {
		create_Kill();
	}
	else {
		this->nrOfActive.Kills++;
	}	
	return this->GameObjManager->getGameObject("Kill"+std::to_string(index));
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

GameObject*& PowerupManager::get_Card()
{
	int index = this->nrOfActive.Cards;
	if (this->nrOfActive.Cards >= this->nrOf.Cards ) {
		create_Card();
	}
	else {
		this->nrOfActive.Cards++;
	}	
	return this->GameObjManager->getGameObject("Card"+std::to_string(index));
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

void PowerupManager::create_EMP()
{
	std::string name = "EMP" + std::to_string(nrOf.EMPs++);
	nrOfActive.EMPs = nrOf.EMPs;
	powers.push_back(new Powerups(rm->get_Models("EMP.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000.0f, 1000.0f, 1000.0f),
			 vec3(0.0f, 0.0f, 0.0f),
			 vec3(0.2f, 0.2f, 0.2f),
			 EMP));
	
	GameObjManager->addGameObject(powers.back(), name);
	collisionHandler->addPowerups(powers.back());
}

void PowerupManager::create_Pad()
{
	std::string name = "Pad" + std::to_string(nrOf.Pads++);
	nrOfActive.Pads = nrOf.Pads;
	powers.push_back(new Powerups(rm->get_Models("GoldenApple.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000.0f, 1000.0f, 1000.0f),
			 vec3(0.0f, 0.0f, 0.0f),
			 vec3(0.1f, 0.1f, 0.1f),
			 PAD));
	
	GameObjManager->addGameObject(powers.back(), name);
	collisionHandler->addPowerups(powers.back());
}

void PowerupManager::create_Kill()
{
	std::string name = "Kill" + std::to_string(nrOf.Kills++);
	nrOfActive.Kills = nrOf.Kills;
	powers.push_back(new Powerups(rm->get_Models("Skull.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000.0f, 1000.0f, 1000.0f),
			 vec3(0.0f, 0.0f, 0.0f),
			 vec3(0.1f, 0.1f, 0.1f),
			 KILL));
	
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

void PowerupManager::create_Card()
{
	std::string name = "Card" + std::to_string(nrOf.Cards++);
	nrOfActive.Cards = nrOf.Cards;
	powers.push_back(new Powerups(rm->get_Models("Rocket.obj", gfx), gfx, player, ghost, mouse, keyboard,
			 vec3(1000.0f, 1000.0f, 1000.0f),
			 vec3(0.0f, deg_to_rad(30.f), deg_to_rad(-30.f)),
			 vec3(0.1f, 0.1f, 0.1f),
			 CARD));
	
	GameObjManager->addGameObject(powers.back(), name);
	collisionHandler->addPowerups(powers.back());
}

void PowerupManager::setUpPowerups(int chosenDiff, vec3 pos)
{
	int chosenPower = 1 + (rand() % 100);
	//Difficulty easy
	if (chosenDiff == 1)
	{
		if (chosenPower <= 60)
		{
			//Got F-tier
			if (chosenPower <= 15)
			{
				//choose feather and moved into position
				get_Feather()->setPos(pos);

			}
			else if (chosenPower > 15 && chosenPower <= 30)
			{
				//choose speed and moved into position
				get_Potion()->setPos(pos);

			}
			else if (chosenPower > 30 && chosenPower <= 45)
			{
				//choose shield and moved into position
				get_Shield()->setPos(pos);
			}
			else if (chosenPower > 45 && chosenPower <= 60)
			{
				// choose Money and moved into position
				get_Money()->setPos(pos);
			}
		}
		else if (chosenPower > 60 && chosenPower <= 92)
		{
			//Got C-tier
			if (chosenPower > 60 && chosenPower <= 76)
			{
				//Freeze
				get_Snowflake()->setPos(pos);
			}
			else if (chosenPower > 76 && chosenPower <= 92)
			{
				//Pearl
				get_Pearl()->setPos(pos);
			}
			//else if (chosenPower > 76 && chosenPower <= 84)
			//{
			//	// EMP
			//	get_EMP()->setPos(pos);
			//}
			//else if (chosenPower > 84 && chosenPower <= 92)
			//{
			//	// PAD
			//	get_Pad()->setPos(pos);
			//}
		}
		else if (chosenPower >= 93)
		{
			//Got S-tier
			if (chosenPower >= 93 && chosenPower <= 96)
			{
				//Apple
				get_GoldenApple()->setPos(pos);
			}
			//else if (chosenPower > 94 && chosenPower <= 96)
			//{
			//	//Kill
			//	get_Kill()->setPos(pos);
			//}
			else if (chosenPower > 96 && chosenPower <= 100)
			{
				// Rocket
				get_Rocket()->setPos(pos);
			}
			//else if (chosenPower > 98 && chosenPower <= 100)
			//{
			//	// Card
			//	//get_Card()->setPos(pos);
			//}
		}
	}
	//else if (chosenDiff == 2) //Difficult medium
	//{
	//	if (chosenPower <= 36)
	//	{
	//		//Got F-tier
	//		if (chosenPower <= 9)
	//		{
	//			//choose feather and moved into position
	//			get_Feather()->setPos(pos);

	//		}
	//		else if (chosenPower > 9 && chosenPower <= 18)
	//		{
	//			//choose speed and moved into position
	//			get_Potion()->setPos(pos);

	//		}
	//		else if (chosenPower > 18 && chosenPower <= 27)
	//		{
	//			//choose shield and moved into position
	//			get_Shield()->setPos(pos);
	//		}
	//		else if (chosenPower > 27 && chosenPower <= 36)
	//		{
	//			// choose Money and moved into position
	//			get_Money()->setPos(pos);
	//		}
	//	}
	//	else if (chosenPower > 36 && chosenPower <= 84)
	//	{
	//		//Got C-tier
	//		if (chosenPower >= 37 && chosenPower <= 48)
	//		{
	//			//Freeze
	//			get_Snowflake()->setPos(pos);
	//		}
	//		else if (chosenPower > 48 && chosenPower <= 60)
	//		{
	//			//Pearl
	//			get_Pearl()->setPos(pos);
	//		}
	//		else if (chosenPower > 60 && chosenPower <= 72)
	//		{
	//			// EMP
	//			get_EMP()->setPos(pos);
	//		}
	//		else if (chosenPower > 72 && chosenPower <= 84)
	//		{
	//			// PAD
	//			get_Pad()->setPos(pos);
	//		}
	//	}
	//	else if (chosenPower >= 85)
	//	{
	//		//Got S-tier
	//		if (chosenPower >= 85 && chosenPower <= 88)
	//		{
	//			//Freeze
	//			get_GoldenApple()->setPos(pos);
	//		}
	//		else if (chosenPower > 88 && chosenPower <= 92)
	//		{
	//			//Pearl
	//			get_Kill()->setPos(pos);
	//		}
	//		else if (chosenPower > 92 && chosenPower <= 96)
	//		{
	//			// EMP
	//			get_Rocket()->setPos(pos);
	//		}
	//		else if (chosenPower > 96 && chosenPower <= 100)
	//		{
	//			// PAD
	//			//get_Pad()->setPos(pos);
	//		}
	//	}
	//}
	//else if (chosenDiff == 3)
	//{
	//	if (chosenPower <= 24)
	//	{
	//		//Got F-tier
	//		if (chosenPower <= 6)
	//		{
	//			//choose feather and moved into position
	//			get_Feather()->setPos(pos);

	//		}
	//		else if (chosenPower > 6 && chosenPower <= 12)
	//		{
	//			//choose speed and moved into position
	//			get_Potion()->setPos(pos);

	//		}
	//		else if (chosenPower > 12 && chosenPower <= 18)
	//		{
	//			//choose shield and moved into position
	//			get_Shield()->setPos(pos);
	//		}
	//		else if (chosenPower > 18 && chosenPower <= 24)
	//		{
	//			// choose Money and moved into position
	//			get_Money()->setPos(pos);
	//		}
	//	}
	//	else if (chosenPower > 24 && chosenPower <= 60)
	//	{
	//		//Got C-tier
	//		if (chosenPower >= 24 && chosenPower <= 33)
	//		{
	//			//Freeze
	//			get_Snowflake()->setPos(pos);
	//		}
	//		else if (chosenPower > 33 && chosenPower <= 42)
	//		{
	//			//Pearl
	//			get_Pearl()->setPos(pos);
	//		}
	//		else if (chosenPower > 42 && chosenPower <= 51)
	//		{
	//			// EMP
	//			get_EMP()->setPos(pos);
	//		}
	//		else if (chosenPower > 51 && chosenPower <= 60)
	//		{
	//			// PAD
	//			get_Pad()->setPos(pos);
	//		}
	//	}
	//	else if (chosenPower > 61)
	//	{
	//		//Got S-tier
	//		if (chosenPower >= 61 && chosenPower <= 70)
	//		{
	//			//GoldenApple
	//			get_GoldenApple()->setPos(pos);
	//		}
	//		else if (chosenPower > 70 && chosenPower <= 80)
	//		{
	//			//Kill
	//			get_Kill()->setPos(pos);
	//		}
	//		else if (chosenPower > 80 && chosenPower <= 90)
	//		{
	//			// Rocket
	//			get_Rocket()->setPos(pos);
	//		}
	//		else if (chosenPower > 90 && chosenPower <= 100)
	//		{
	//			// Card
	//			get_Card()->setPos(pos);
	//		}
	//	}
	//}
}
