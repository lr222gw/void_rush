#include "puzzle_memory.hpp"

MemoryPuzzle::MemoryPuzzle(int seed, Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager) : Puzzle(seed, gfx, rm, colHandler, soundManager)
{
}

void MemoryPuzzle::Interaction(vec3 playerPos, vec3 forwardVec)
{
    if (!this->GetState())
    {
        float size, test;
        vec3 midPos;
        /*
        midPos = GetMidPos(puzzleObjects[0], size);
        if (CanInteract(playerPos, forwardVec, midPos, size / 2.0f, 1.5f, test))
        {
            soundManager->playSound("Correct", midPos);
            for (size_t i = 0; i < puzzleObjects.size(); i++) {
                delete puzzleObjects[i];
            }
            puzzleObjects.clear();
            vec3 spawnPosition = vec3(puzzlePlatform->getxPos(), puzzlePlatform->getyPos(), puzzlePlatform->getzPos() + (puzzlePlatform->getWidthHeightDepth().z / 2.0f));
            this->SpawnDoor(spawnPosition);
        }
        */
    }
    else
    {
        this->InteractPortal(playerPos, forwardVec);
    }
}

void MemoryPuzzle::InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position)
{
    this->SetPosition(position);
    std::cout << "Memory puzzle chosen" << std::endl;
    puzzlePlatform = new GameObject(rm->get_Models("BasePlatformTexture.obj", gfx), gfx, position, vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));

    this->GetColHandler()->addPlatform(puzzlePlatform);
}

void MemoryPuzzle::Update(Graphics*& gfx)
{
    puzzlePlatform->updateMatrix();
    puzzlePlatform->update(0);
    puzzlePlatform->updateVertexShader(gfx);
    puzzlePlatform->updatePixelShader(gfx);
    puzzlePlatform->draw(gfx);

    if (!this->GetState())
    {
        for (int i = 0; i < puzzleObjects.size(); i++) {

            puzzleObjects[i]->updateMatrix();
            puzzleObjects[i]->update(0);
            puzzleObjects[i]->updateVertexShader(gfx);
            puzzleObjects[i]->updatePixelShader(gfx);
            puzzleObjects[i]->draw(gfx);
        }
    }
    else
    {
        this->UpdatePortal();
    }
}
