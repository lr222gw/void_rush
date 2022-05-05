#include "puzzle_hidden.hpp"

HiddenPuzzle::HiddenPuzzle(int seed,Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager) : Puzzle(seed, gfx, rm, colHandler, soundManager)
{
    
}

void HiddenPuzzle::Interaction(vec3 playerPos, vec3 forwardVec)
{
    //Check if left click is pressed while player is close enough and looking at the hitbox of the key.
    if (!this->GetState())
    {
        float size, test;
        vec3 midPos;
        midPos = GetMidPos(puzzleObjects[0], size);
        if (CanInteract(playerPos, forwardVec, midPos, size / 2.0f, 5.0f, test))
        {
            soundManager->playSound("Correct", midPos);
            for (size_t i = 0; i < puzzleObjects.size(); i++) {
                this->GetColHandler()->deletePlatform(puzzleObjects[i]);
                delete puzzleObjects[i];
            }
            puzzleObjects.clear();
            std::cout << "Key Picked Up!" << std::endl;
            this->SpawnDoor(this->GetPosition());
        }
    }
    else
    {
        this->InteractPortal(playerPos, forwardVec);
    }
}

void HiddenPuzzle::InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position)
{
    this->SetPosition(position);
    std::cout << "Hidden puzzle chosen" << std::endl;
    //Based on the size of the platform, place out the key in a valid position where the key can be reached by the player.
    puzzlePlatform = new GameObject(rm->get_Models("BasePlatform.obj", gfx), gfx, position, vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));
    float x, z;
    x = (float)(rand() % (int)puzzlePlatform->getWidthHeightDepth().x - 3);
    z = (float)(rand() % (int)puzzlePlatform->getWidthHeightDepth().z - 2);

    if (x > (puzzlePlatform->getWidthHeightDepth().x / 2.0f))
    {
        x = x - (puzzlePlatform->getWidthHeightDepth().x - 3.0f);
    }

    if (z > (puzzlePlatform->getWidthHeightDepth().z / 2.0f))
    {
        z = z - (puzzlePlatform->getWidthHeightDepth().z - 2.0f);
    }

    puzzleObjects.push_back(new GameObject(rm->get_Models("Key.obj", gfx), gfx, vec3(puzzlePlatform->getxPos() + x, puzzlePlatform->getyPos() + 0.2f, puzzlePlatform->getzPos() + z), vec3(x, x / 2, z), vec3(1.0f, 1.0f, 1.0f)));

    this->GetColHandler()->addPlatform(puzzlePlatform);
    this->GetColHandler()->addPlatform(puzzleObjects[0]);
}

void HiddenPuzzle::Update(Graphics*& gfx)
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
