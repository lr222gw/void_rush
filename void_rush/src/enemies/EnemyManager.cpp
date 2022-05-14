#include "EnemyManager.hpp"
vec3 EnemyManager::FarFarFarAway = vec3(1000.f, 1000.f, 1000.f);
void EnemyManager::createTurret()
{
	//GameObjManager->CreateGameObject("Bullet.obj", "bull", FarFarFarAway);
	
	std::string name = "Turret" + std::to_string(nrOf.TURRET++);
	this->nrOfActive.TURRET = this->nrOf.TURRET;

	GameObjManager->CreateEnemy(player, enemyType::TURRET, *soundManager, "Turret.obj", name, FarFarFarAway);
	const int MaxNrOfProjectiles = enemy_conf.Turret_maxNrOfProjectiles;
	for (int i = 0; i < MaxNrOfProjectiles; i++) {

		std::string name_proj = name + "_proj" + std::to_string(i);
		GameObjManager->CreateEnemy(player, enemyType::PROJECTILE, *soundManager, 
			"Bullet.obj", name_proj,
			FarFarFarAway,
			vec3(0, 0, 0), 
			vec3(0.4f, 0.4f, 0.4f));

		collisionHandler->addEnemies((Enemy*)GameObjManager->getGameObject(name_proj));
		((Turret*)GameObjManager->getGameObject(name))->addProjectiles((TurrProjectile*)GameObjManager->getGameObject(name_proj));
	}
	enemies.push_back(static_cast<Enemy*>(this->GameObjManager->getGameObject(name)));
}

void EnemyManager::createSpikes()
{
	std::string name = "spikes" + std::to_string(nrOf.SPIKES++);
	this->nrOfActive.SPIKES = this->nrOf.SPIKES;

	GameObjManager->CreateEnemy(player, enemyType::SPIKES, *soundManager, "Spikes.obj", name,
		FarFarFarAway,
		vec3(0.0f, 0.0f, 0.0f), 
		vec3(0.0001f, 0.0001f, 0.0001f), 
		true);
	collisionHandler->addEnemies((Enemy*)GameObjManager->getGameObject(name));
	enemies.push_back(static_cast<Enemy*>(this->GameObjManager->getGameObject(name)));
}

void EnemyManager::createSnare()
{
	std::string name = "snare" + std::to_string(nrOf.SNARE++);
	this->nrOfActive.SNARE = this->nrOf.SNARE;
	GameObjManager->CreateEnemy(player, enemyType::SNARE, *soundManager, "DCube.obj", name,
		FarFarFarAway,
		vec3(0.0f, 0.0f, 0.0f), 
		vec3(0.3f, 0.2f, 0.3f));
	collisionHandler->addEnemies((Enemy*)GameObjManager->getGameObject(name));
	enemies.push_back(static_cast<Enemy*>(this->GameObjManager->getGameObject(name)));
}

Enemy* EnemyManager::get_Turret()
{
	int index = this->nrOfActive.TURRET;
	if (this->nrOfActive.TURRET >= this->nrOf.TURRET) {
		createTurret();
	}
	else {
		this->nrOfActive.TURRET++;
	}
	return static_cast<Enemy*>(this->GameObjManager->getGameObject("Turret" + std::to_string(index)));
}

Enemy* EnemyManager::get_Spikes()
{
	int index = this->nrOfActive.SPIKES;
	if (this->nrOfActive.SPIKES >= this->nrOf.SPIKES) {
		createSpikes();
	}
	else {
		this->nrOfActive.SPIKES++;
	}
	return static_cast<Enemy*>(this->GameObjManager->getGameObject("spikes" + std::to_string(index)));
}

Enemy* EnemyManager::get_Snare()
{
	int index = this->nrOfActive.SNARE;
	if (this->nrOfActive.SNARE >= this->nrOf.SNARE) {
		createSnare();
	}
	else {
		this->nrOfActive.SNARE++;
	}
	return static_cast<Enemy*>(this->GameObjManager->getGameObject("snare" + std::to_string(index)));
}

EnemyManager::EnemyManager(GameObjectManager* GameObjManager, Graphics* gfx, ResourceManager* rm, CollisionHandler* collisionHandler, SoundManager* soundManager, Mouse* mouse, Keyboard* keyboard)
	:  GameObjManager(GameObjManager), gfx(gfx), rm(rm), collisionHandler(collisionHandler), keyboard(keyboard), mouse(mouse), soundManager(soundManager),
	player(nullptr), ghost(nullptr)
{
}

void EnemyManager::init(Player* player, Ghost* ghost)
{
	this->player = player;
	this->ghost = ghost;
}

void EnemyManager::reset()
{
	nrOfActive = nrOfEnemies();
	for (auto& e : this->enemies) {
		e->setPos(FarFarFarAway);
	}
}

void EnemyManager::spawnEnemy(vec3 pos)
{
	int enemyType = rand() % (int)respawnable_enemyType::_COUNT ;

	switch(enemyType){
		case (int)respawnable_enemyType::TURRET:
			pos.y = pos.y + enemy_conf.Turret_y_Offset;
			get_Turret()->setPos(pos);
			break;
		case (int)respawnable_enemyType::SPIKES:
			pos.y = pos.y + enemy_conf.Spikes_y_Offset; 
			get_Spikes()->setPos(pos);
			break;
		case (int)respawnable_enemyType::SNARE:
			pos.y = pos.y + enemy_conf.Snare_y_Offset;
			get_Snare()->setPos(pos);
			break;		
		default:
			std::cout << "Shit" << std::endl;
	}

}
