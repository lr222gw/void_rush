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

void GameObjectManager::addGameObject(GameObject* obj, std::string name)
{
	gameObjects.insert({ name, obj });
	VGameObj.push_back(obj);
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

void GameObjectManager::update()
{
	for (auto const& [key, val] : gameObjects) {
		val->update();
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
