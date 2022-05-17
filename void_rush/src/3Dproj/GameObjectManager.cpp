#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(Graphics*& gfx, ResourceManager*& rm)
{
	this->rm = rm;
	this->gfx = gfx;
	GameObjID = 0;
}

GameObjectManager::~GameObjectManager()
{
	for (auto const& [key, val] : gameObjects) {
		delete val;
	}
}

void GameObjectManager::addGameObject(GameObject* obj, std::string name, bool interactable)
{
	gameObjects.insert({ name, obj });
	VGameObj.push_back(obj);

	if(interactable)
		VInteractGameObj.push_back(obj);
}

void GameObjectManager::addInteractGameObject(GameObject* obj)
{
	VInteractGameObj.push_back(obj);
}

void GameObjectManager::removeGameObject(std::string name, bool del)
{
	std::map<std::string, GameObject*>::iterator it;
	it = gameObjects.find(name);
	for (int i = 0; i < VGameObj.size(); i++) {
		if (VGameObj[i] == it->second) {
			VGameObj.erase(std::next(VGameObj.begin(),i));
		}
	}
	if (del) {
		delete it->second;
	}
	gameObjects.erase(it);
	
	CleanUpInteractables();

}


void GameObjectManager::CreateGameObject(std::string modelFile, std::string name, vec3 pos, vec3 rot, vec3 scale)
{
	if (name == "") {
		name = "_model_id_name_" + std::to_string(++GameObjID);
	}
	GameObject* objPtr = new GameObject(rm->get_Models(modelFile, gfx), gfx, pos, rot, scale);
	gameObjects.insert({ name, objPtr });
	VGameObj.push_back(objPtr);
}

void GameObjectManager::CreateEnemy(Player* player, enemyType typeofEnemy, SoundManager& sm, std::string modelFile, std::string name, vec3 pos, vec3 rot, vec3 scale, bool moving)
{
	if (name == "") {
		name = "_model_id_name_" + std::to_string(++GameObjID);
	}
	GameObject* objPtr;
	switch (typeofEnemy)
	{
	case enemyType::TURRET:
		if (modelFile == "") {
			modelFile = "Turret.obj";
		}
		objPtr = new Turret(rm->get_Models(modelFile, gfx), gfx, player, pos, rot, scale);
		((Turret*)objPtr)->setSoundManager(sm);
		break;
	case enemyType::PROJECTILE:
		if (modelFile == "") {
			modelFile = "DCube.obj";
		}
		objPtr = new TurrProjectile(rm->get_Models(modelFile, gfx), gfx, player, pos, rot, scale);
		break;
	case enemyType::SPIKES:
		if (modelFile == "") {
			modelFile = "Spikes.obj";
		}
		objPtr = new Spikes(rm->get_Models(modelFile, gfx), gfx, player, pos, rot, scale, moving);
		break;
	case enemyType::SNARE:
		if (modelFile == "") {
			modelFile = "snareTest.obj";
		}
		objPtr = new Snare(rm->get_Models(modelFile, gfx), gfx, player, pos, rot, scale);
		break;
	case enemyType::MUSHROOM:
		if (modelFile == "") {
			modelFile = "DCube.obj";
		}
		objPtr = new Mushroom(rm->get_Models(modelFile, gfx), gfx, player, pos, rot, scale);
		break;
	case enemyType::FALLPLAT:
		if (modelFile == "") {
			modelFile = "DCube.obj";
		}
		objPtr = new FallingPlatform(rm->get_Models(modelFile, gfx), gfx, player, pos, rot, scale);
		break;
	default:
		break;
	}
	gameObjects.insert({ name, objPtr });
	VGameObj.push_back(objPtr);
}


std::vector<GameObject*>& GameObjectManager::getAllGameObjects()
{
	return VGameObj;
}

std::vector<GameObject*>& GameObjectManager::getAllInteractGameObjects()
{
	return VInteractGameObj;
}
void GameObjectManager::update(float dt)
{
	for (auto const& [key, val] : gameObjects) {
		val->update(dt);
	}
}

void GameObjectManager::updateVertex()
{
	for (auto const& [key, val] : gameObjects) {
		val->updateVertexShader(gfx);
	}
}

void GameObjectManager::updatePixel()
{
	for (auto const& [key, val] : gameObjects) {
		val->updatePixelShader(gfx);
	}
}

void GameObjectManager::updateMatrix()
{
	for (auto const& [key, val] : gameObjects) {
		val->updateMatrix();
	}
}

void GameObjectManager::draw()
{
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[0]);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);

	for (auto const& [key, val] : gameObjects) {
		val->draw(gfx, false);
	}
}

void GameObjectManager::drawShadow()
{
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx->get_IMctx()->GSSetShader(nullptr, nullptr, 0);
	gfx->get_IMctx()->PSSetShader(nullptr, nullptr, 0);
	for (auto const& [key, val] : gameObjects) {
		val->draw(gfx, true);
	}
}

GameObject*& GameObjectManager::getGameObject(std::string key)
{
	return gameObjects.find(key)->second;
}

GameObject*& GameObjectManager::getGameObject(int index)
{
	return VGameObj[index];
}

void GameObjectManager::CleanUpInteractables()
{
	std::vector<GameObject*>temp;
	for (int i = 0; i < VInteractGameObj.size(); i++) {
		if (VInteractGameObj[i] != nullptr) {
			temp.push_back(VInteractGameObj[i]);
		}
	}
	VInteractGameObj = temp;
}
