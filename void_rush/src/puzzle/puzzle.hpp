#pragma once

//Add includes here
#include "../common/Vector.hpp"
#include <random>
#include <string>
#include <iostream>
#include "3Dproj/GameObject.h"
#include "3Dproj/ResourceManager.h"
#include "3Dproj/Graphics.h"
#include "Portal.h"
#include <vector>

#include "../3Dproj/GameObjectManager.h"
#include "../3Dproj/CollisionHandler.h"
#include "../generation/Generation_manager.hpp"

class Puzzle : public Portal
{
private:

protected:
    int seed;
    std::vector<GameObject*> puzzleObjects;
    GameObject* puzzlePlatform = nullptr;


public:
    //Create puzzle
    Puzzle(int seed, Graphics*& gfx, ResourceManager*& rm, Generation_manager*& generationManager, CollisionHandler* collHandl);
    virtual ~Puzzle();

    bool GetState() const;

    void ResetState();

    int GetSeed() const;

    void SpawnDoor();

    virtual void Interaction(vec3 playerPos, vec3 forwardVec) = 0;

    //Pick the correct type of puzzle and initiate it.
    virtual void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position) = 0;

    virtual void Update(Graphics*& gfx) = 0;
};