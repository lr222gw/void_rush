#pragma once
#include "GameObject.h"
#include <map>
#include "ResourceManager.h"
#include "Light.h"

class GameObjectManager {
public:
	GameObjectManager(Graphics*& gfx, ResourceManager*& rm);
	virtual ~GameObjectManager();
	void addGameObject(GameObject* obj, std::string name);
	//cannot remove a object that ha
	void removeGameObject(std::string name, bool del = true);
	void CreateGameObject(std::string modelFile, std::string name = "", vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1));
	std::vector<GameObject*>& getAllGameObjects();
	void update();
	void updateVertex();
	void updatePixel();
	void updateMatrix();
	void draw();
	void drawShadow();
	GameObject*& getGameObject(std::string key);
	GameObject*& getGameObject(int index);
private:
	Graphics* gfx;
	ResourceManager* rm;
	int GameObjID;
	std::vector<GameObject*> VGameObj;
	std::map<std::string, GameObject*> gameObjects;

};
