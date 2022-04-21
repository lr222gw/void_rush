#include "puzzle_hidden.hpp"

HiddenPuzzle::HiddenPuzzle(int seed,Graphics*& gfx, ResourceManager*& rm) : Puzzle(seed, gfx, rm)
{
    
}

void HiddenPuzzle::Interaction(vec3 playerPos, vec3 forwardVec)
{
    //Check if left click is pressed while player is close enough and looking at the hitbox of the key.
    if (!this->GetState())
    {
        float test;
        if (CanInteract(playerPos, forwardVec, puzzleObjects[0]->getPos(), 5.0f, 5.0f, test))
        {
            std::cout << "Key Picked Up!" << std::endl;
            this->SpawnDoor();
        }
        /*
        if ((puzzleObjects[0]->getPos() - playerPos).length() < 10.0f)
        {
            std::cout << "Key Picked Up!" << std::endl;
            this->SpawnDoor();
        }
        */
    }
    else
    {
        this->InteractPortal(playerPos, forwardVec);
    }
}

void HiddenPuzzle::InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position)
{
    std::cout << "Hidden puzzle chosen" << std::endl;
    //Based on the size of the platform, place out the key in a valid position where the key can be reached by the player.
    puzzlePlatform = new GameObject(rm->get_Models("BasePlatform.obj", gfx), gfx, position, vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));
    float x, y, z;
    x = (float)(rand() % (int)puzzlePlatform->getWidthHeightDepth().x - 1);
    y = puzzlePlatform->getWidthHeightDepth().y;
    z = (float)(rand() % (int)puzzlePlatform->getWidthHeightDepth().z - 1);

    if (x > (puzzlePlatform->getWidthHeightDepth().x / 2.0f))
    {
        x = x - (puzzlePlatform->getWidthHeightDepth().x - 1.0f);
    }

    if (z > (puzzlePlatform->getWidthHeightDepth().z / 2.0f))
    {
        z = z - (puzzlePlatform->getWidthHeightDepth().z - 1.0f);
    }

    std::cout << puzzlePlatform->getWidthHeightDepth().x << " " << puzzlePlatform->getWidthHeightDepth().y << " " << puzzlePlatform->getWidthHeightDepth().z << std::endl;

    puzzleObjects.push_back(new GameObject(rm->get_Models("Key.obj", gfx), gfx, vec3(puzzlePlatform->getxPos() + x, puzzlePlatform->getyPos() + y, puzzlePlatform->getzPos() + z), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));
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
        this->UpdatePortal(gfx);
    }
}
