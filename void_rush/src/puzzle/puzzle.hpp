#pragma once

//Add includes here
#include <random>
#include <string>
#include <iostream>
#include "3Dproj/GameObject.h"
#include "3Dproj/ResourceManager.h"
#include "3Dproj/Graphics.h"
#include "3Dproj/SoundManager.h"
#include "Portal.h"
#include <vector>

class Puzzle : public Portal
{
private:

protected:
    int seed;
    std::vector<GameObject*> puzzleObjects;
    GameObject* puzzlePlatform = nullptr;
    vec3 position = vec3(0.0f, 0.0f, 0.0f);
    SoundManager* soundManager;
    vec3 playerPos = vec3(0.0f, 0.0f, 0.0f);
    float deltaT = 0.0f;

public:
    //Create puzzle
    Puzzle(int seed, Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager);
    virtual ~Puzzle();

    void SetPosition(vec3 pos);

    vec3 GetPosition() const;

    bool GetState() const;

    CollisionHandler* GetColHandler();

    void ResetState();

    int GetSeed() const;

    bool GetPortalCompleted() const;

    void SpawnDoor(vec3 pos);

    void UpdatePlayerPos(vec3 newPlayerPos);

    void UpdateDT(float dt);

    float getDT();

    virtual void Interaction(vec3 playerPos, vec3 forwardVec) = 0;

    //Pick the correct type of puzzle and initiate it.
    virtual void InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position) = 0;

    virtual void Update(Graphics*& gfx) = 0;
};