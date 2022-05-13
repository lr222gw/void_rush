#pragma once
#include "Player.h"
#include "Turret.h"
#include "Ghost.h"
#include "Spikes.h"
#include "Snare.h"
#include "Mushroom.h"
#include <map>
#include "ResourceManager.h"
#include "Light.h"

class GameObjectManager {
public:
	GameObjectManager(Graphics*& gfx, ResourceManager*& rm);
	virtual ~GameObjectManager();
	void addGameObject(GameObject* obj, std::string name, bool interactable = false);
	void addInteractGameObject(GameObject* obj);
	//cannot remove a object that ha
	void removeGameObject(std::string name, bool del = true);
	void CreateGameObject(std::string modelFile, std::string name = "", vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	void CreateEnemy(Player* player, enemyType typeofEnemy, SoundManager& sm, std::string modelFile = "", std::string name = "", vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), bool moving = false);
	std::vector<GameObject*>& getAllGameObjects();
	std::vector<GameObject*>& getAllInteractGameObjects();
	void update(float dt);
	void updateVertex();
	void updatePixel();
	void updateMatrix();
	void draw();
	void drawShadow();
	GameObject*& getGameObject(std::string key);
	GameObject*& getGameObject(int index);
private:
	void CleanUpInteractables();
private:
	Graphics* gfx;
	ResourceManager* rm;
	int GameObjID;
	std::vector<GameObject*> VGameObj;
	std::map<std::string, GameObject*> gameObjects;

	std::vector<GameObject*>VInteractGameObj;

};
