#include "puzzle_move.hpp"

MovePuzzle::MovePuzzle(int seed, Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager) : Puzzle(seed, gfx, rm, colHandler, soundManager)
{
}

void MovePuzzle::Interaction(vec3 playerPos, vec3 forwardVec)
{
    if (!this->GetState())
    {
        float size, test;
        vec3 midPos;
        midPos = GetMidPos(puzzleObjects[0], size);
        if (CanInteract(playerPos, forwardVec, midPos, size / 2.0f, 1.5f, test))
        {
            speedDir = vec2(puzzleObjects[0]->getxPos(), puzzleObjects[0]->getzPos()) - vec2(playerPos.x, playerPos.z);
            speedDir.Normalize();
            std::cout << speedDir.x << " " << speedDir.y << std::endl;
            speed = 0.02f;
        }
    }
    else
    {
        this->InteractPortal(playerPos, forwardVec);
    }
}

void MovePuzzle::InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position)
{
    this->SetPosition(position);
    std::cout << "Move puzzle chosen" << std::endl;

    puzzlePlatform = new GameObject(rm->get_Models("BasePlatformTexture.obj", gfx), gfx, position, vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));

    startPos = vec3(puzzlePlatform->getxPos(), puzzlePlatform->getyPos() + 5.0f, puzzlePlatform->getzPos());
    puzzleObjects.push_back(new GameObject(rm->get_Models("BigRock.obj", gfx), gfx, startPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.1f, 0.1f, 0.1f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("BigRock.obj", gfx), gfx, this->puzzlePlatform->getPos() + vec3(5.0f, 4.8f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.1f, 0.1f, 0.1f)));

    this->GetColHandler()->addPlatform(puzzlePlatform);
}

void MovePuzzle::Update(Graphics*& gfx)
{
    puzzlePlatform->updateMatrix();
    puzzlePlatform->update(0);
    puzzlePlatform->updateVertexShader(gfx);
    puzzlePlatform->updatePixelShader(gfx);
    puzzlePlatform->draw(gfx);

    if (!this->GetState())
    {
        puzzleObjects[0]->movePos(vec3(speedDir.x, 0.0f, speedDir.y) * speed);

        if (puzzleObjects[0]->getxPos() >= (this->puzzlePlatform->getxPos() + this->puzzlePlatform->getWidthHeightDepth().x / 2) ||
            puzzleObjects[0]->getxPos() <= (this->puzzlePlatform->getxPos() - this->puzzlePlatform->getWidthHeightDepth().x / 2) ||
            puzzleObjects[0]->getzPos() >= (this->puzzlePlatform->getzPos() + this->puzzlePlatform->getWidthHeightDepth().z / 2) ||
            puzzleObjects[0]->getzPos() <= (this->puzzlePlatform->getzPos() - this->puzzlePlatform->getWidthHeightDepth().z / 2))
        {
            puzzleObjects[0]->setPos(startPos);
            this->speed = 0;
        }
        for (int i = 0; i < puzzleObjects.size(); i++) {

            puzzleObjects[i]->updateMatrix();
            puzzleObjects[i]->update(0);
            puzzleObjects[i]->updateVertexShader(gfx);
            puzzleObjects[i]->updatePixelShader(gfx);
            puzzleObjects[i]->draw(gfx);
        }

        if (sqrtf(powf(fabs(puzzleObjects[0]->getxPos()) - fabs(puzzleObjects[1]->getxPos()), 2.0f) + powf(fabs(puzzleObjects[0]->getzPos()) - fabs(puzzleObjects[1]->getzPos()), 2.0f)) <= 0.5f)
        {
            soundManager->playSound("Correct", puzzleObjects[0]->getPos());
            for (size_t i = 0; i < puzzleObjects.size(); i++) {
                delete puzzleObjects[i];
            }
            puzzleObjects.clear();
            vec3 spawnPosition = vec3(puzzlePlatform->getxPos(), puzzlePlatform->getyPos(), puzzlePlatform->getzPos() + (puzzlePlatform->getWidthHeightDepth().z / 2.0f));
            this->SpawnDoor(spawnPosition);
        }
    }
    else
    {
        this->UpdatePortal();
    }
}
