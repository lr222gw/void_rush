#include "puzzle_memory.hpp"

MemoryPuzzle::MemoryPuzzle(int seed, Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager) : Puzzle(seed, gfx, rm, colHandler, soundManager), letters(new Letters3DHandler(rm, gfx)), answerLetters(new Letters3DHandler(rm, gfx))
{
}

MemoryPuzzle::~MemoryPuzzle()
{
    letters->removeAllText();
    delete letters;

    answerLetters->removeAllText();
    delete answerLetters;
}

void MemoryPuzzle::Interaction(vec3 playerPos, vec3 forwardVec)
{
    if (!this->GetState())
    {
        float size, test;
        vec3 midPos;
        std::string text;
        bool spawnDoor = false;
        for (int i = 0; i < 10; i++)
        {
            midPos = GetMidPos(puzzleObjects[i], size);
            if (CanInteract(playerPos, forwardVec, midPos, size / 2.0f, 1.5f, test))
            {
                if (answer[currentNumber] == i)
                {
                    currentAnswer[currentNumber] = i;
                    if (currentNumber == 0)
                    {
                        soundManager->playSound("Correct", midPos);
                        text = std::to_string(i) + "__";
                    }
                    else if (currentNumber == 1)
                    {
                        soundManager->playSound("Correct", midPos);
                        text = std::to_string(this->answer[0]) + std::to_string(i) + "_";
                    }
                    else if (currentNumber == 2)
                    {
                        soundManager->playSound("Correct", midPos);
                        spawnDoor = true;
                    }
                    this->currentNumber++;
                }
                else
                {
                    soundManager->playSound("Wrong", midPos);
                    text = "___";
                    this->currentNumber = 0;
                }
                letters->editText("Choice", text);
            }
        }

        if(spawnDoor == true)
        {
            for (size_t i = 0; i < puzzleObjects.size(); i++) {
                delete puzzleObjects[i];
            }
            puzzleObjects.clear();
            this->currentNumber = 0;
            vec3 spawnPosition = vec3(puzzlePlatform->getxPos(), puzzlePlatform->getyPos(), puzzlePlatform->getzPos() + (puzzlePlatform->getWidthHeightDepth().z / 2.0f));
            this->SpawnDoor(spawnPosition);
        }
    }
    else
    {
        this->InteractPortal(playerPos, forwardVec);
    }
}

void MemoryPuzzle::InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position, ExtraArgs extraArgs)
{
    letters->removeAllText();
    answerLetters->removeAllText();

    this->SetPosition(position);
    std::cout << "Memory puzzle chosen" << std::endl;

    this->answer[0] = (int)rand() % 10;
    this->answer[1] = (int)rand() % 10;
    this->answer[2] = (int)rand() % 10;
    vec3 answerPos = extraArgs.nextPlatformPosition + vec3(0.0f, 1.0f, 0.0f);
    answerLetters->createText(std::to_string(this->answer[0]) + std::to_string(this->answer[1]) + std::to_string(this->answer[2]), answerPos, vec2(0.7f, 0.7f), "Answer");
    puzzlePlatform = new GameObject(rm->get_Models("BasePlatformTexture.obj", gfx), gfx, position, vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));

    puzzleObjects.push_back(new GameObject(rm->get_Models("Number0.obj", gfx), gfx, vec3(puzzlePlatform->getxPos(), puzzlePlatform->getyPos() + 5.1f, puzzlePlatform->getzPos() - 4.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.3f, 0.3f, 0.3f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("Number1.obj", gfx), gfx, vec3(puzzlePlatform->getxPos() - 1.5f, puzzlePlatform->getyPos() + 5.1f, puzzlePlatform->getzPos()), vec3(0.0f, 0.0f, 0.0f), vec3(0.3f, 0.3f, 0.3f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("Number2.obj", gfx), gfx, vec3(puzzlePlatform->getxPos(), puzzlePlatform->getyPos() + 5.1f, puzzlePlatform->getzPos()), vec3(0.0f, 0.0f, 0.0f), vec3(0.3f, 0.3f, 0.3f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("Number3.obj", gfx), gfx, vec3(puzzlePlatform->getxPos() + 1.5f, puzzlePlatform->getyPos() + 5.1f, puzzlePlatform->getzPos()), vec3(0.0f, 0.0f, 0.0f), vec3(0.3f, 0.3f, 0.3f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("Number4.obj", gfx), gfx, vec3(puzzlePlatform->getxPos() - 1.5f, puzzlePlatform->getyPos() + 5.1f, puzzlePlatform->getzPos() - 1.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.3f, 0.3f, 0.3f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("Number5.obj", gfx), gfx, vec3(puzzlePlatform->getxPos(), puzzlePlatform->getyPos() + 5.1f, puzzlePlatform->getzPos() - 1.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.3f, 0.3f, 0.3f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("Number6.obj", gfx), gfx, vec3(puzzlePlatform->getxPos() + 1.5f, puzzlePlatform->getyPos() + 5.1f, puzzlePlatform->getzPos() - 1.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.3f, 0.3f, 0.3f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("Number7.obj", gfx), gfx, vec3(puzzlePlatform->getxPos() - 1.5f, puzzlePlatform->getyPos() + 5.1f, puzzlePlatform->getzPos() - 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.3f, 0.3f, 0.3f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("Number8.obj", gfx), gfx, vec3(puzzlePlatform->getxPos(), puzzlePlatform->getyPos() + 5.1f, puzzlePlatform->getzPos() - 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.3f, 0.3f, 0.3f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("Number9.obj", gfx), gfx, vec3(puzzlePlatform->getxPos() + 1.5f, puzzlePlatform->getyPos() + 5.1f, puzzlePlatform->getzPos() - 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.3f, 0.3f, 0.3f)));

    letters->createText("___", vec3(puzzlePlatform->getxPos(), puzzlePlatform->getyPos() + 6.0f, puzzlePlatform->getzPos() + 1.0f), vec2(0.2f, 0.2f), "Choice");

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
        letters->update(this->playerPos);
        letters->draw();
        answerLetters->update();
        answerLetters->draw();
    }
    else
    {
        this->UpdatePortal();
    }
}
