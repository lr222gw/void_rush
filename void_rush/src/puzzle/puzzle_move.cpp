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
            this->moving = true;
        }
    }
    else
    {
        this->InteractPortal(playerPos, forwardVec);
    }
}

void MovePuzzle::InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position, ExtraArgs extraArgs)
{
    this->SetPosition(position);
    std::cout << "Move puzzle chosen" << std::endl;

    puzzlePlatform = new GameObject(rm->get_Models("BasePlatformTexture.obj", gfx), gfx, position, vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));

    startPos = vec3(puzzlePlatform->getxPos(), puzzlePlatform->getyPos() + 5.0f, puzzlePlatform->getzPos());
    puzzleObjects.push_back(new GameObject(rm->get_Models("BigRock.obj", gfx), gfx, startPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.1f, 0.1f, 0.1f)));

    float x, y;
    int xOrY = (int)rand() % 4;

    switch (xOrY)
    {
    case 0:
        x = 5.0f;
        y = rand() % 11 - 5.0f;
    case 1:
        y = 5.0f;
        x = rand() % 11 - 5.0f;
    case 2:
        x = -5.0f;
        y = rand() % 11 - 5.0f;
    case 3:
        y = -5.0f;
        x = rand() % 11 - 5.0f;
    }

    puzzleObjects.push_back(new GameObject(rm->get_Models("Flag.obj", gfx), gfx, this->puzzlePlatform->getPos() + vec3(x, 5.0f, y), vec3(0.0f, 0.0f, 0.0f), vec3(0.1f, 0.1f, 0.1f)));

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
        if (this->moving)
        {
            this->deltaT = this->getDT() * 2.0f;
            puzzleObjects[0]->movePos(vec3(speedDir.x, 0.0f, speedDir.y) * deltaT);

            if (puzzleObjects[0]->getxPos() >= (this->puzzlePlatform->getxPos() + this->puzzlePlatform->getWidthHeightDepth().x / 2) ||
                puzzleObjects[0]->getxPos() <= (this->puzzlePlatform->getxPos() - this->puzzlePlatform->getWidthHeightDepth().x / 2) ||
                puzzleObjects[0]->getzPos() >= (this->puzzlePlatform->getzPos() + this->puzzlePlatform->getWidthHeightDepth().z / 2) ||
                puzzleObjects[0]->getzPos() <= (this->puzzlePlatform->getzPos() - this->puzzlePlatform->getWidthHeightDepth().z / 2))
            {
                puzzleObjects[0]->setPos(startPos);
                this->moving = false;
            }
        }
        for (int i = 0; i < puzzleObjects.size(); i++) {

            puzzleObjects[i]->updateMatrix();
            puzzleObjects[i]->update(0);
            puzzleObjects[i]->updateVertexShader(gfx);
            puzzleObjects[i]->updatePixelShader(gfx);
            puzzleObjects[i]->draw(gfx);
        }

        if (sqrtf(powf(fabs(puzzleObjects[0]->getxPos()) - fabs(puzzleObjects[1]->getxPos()), 2.0f) + powf(fabs(puzzleObjects[0]->getzPos()) - fabs(puzzleObjects[1]->getzPos()), 2.0f)) <= 0.35f)
        {
            soundManager->playSound("Correct", puzzleObjects[0]->getPos());
            for (size_t i = 0; i < puzzleObjects.size(); i++) {
                delete puzzleObjects[i];
            }
            puzzleObjects.clear();
            vec3 spawnPosition = vec3(puzzlePlatform->getxPos(), puzzlePlatform->getyPos(), puzzlePlatform->getzPos() + (puzzlePlatform->getWidthHeightDepth().z / 2.0f));
            this->SpawnDoor(spawnPosition);
            this->moving = false;
        }
    }
    else
    {
        this->UpdatePortal();
    }
}
