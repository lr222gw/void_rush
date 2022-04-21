#include "puzzle_math.hpp"
#include <string>

MathPuzzle::MathPuzzle(int seed, Graphics*& gfx, ResourceManager*& rm) : Puzzle(seed, gfx, rm)
{
}

std::string MathPuzzle::GetComponents() const
{
    return std::to_string(components[0]) + " " + arithmetic + " " + std::to_string(components[1]) + " = " + std::to_string(components[2]) + "\nThese are your choices: " + std::to_string(choices[0]) + " " + std::to_string(choices[1]) + " " + std::to_string(choices[2]);
}

void MathPuzzle::Interaction(vec3 playerPos, vec3 forwardVec)
{
    if (!this->GetState())
    {
        int choice = 3;
        float test;

        if (CanInteract(playerPos, forwardVec, puzzleObjects[0]->getPos(), 5.0f, 5.0f, test))
        {
            choice = 0;
            std::cout << "Is in range A" << std::endl;
        }
        if (CanInteract(playerPos, forwardVec, puzzleObjects[1]->getPos(), 5.0f, 5.0f, test))
        {
            choice = 1;
            std::cout << "Is in range B" << std::endl;
        }
        if (CanInteract(playerPos, forwardVec, puzzleObjects[2]->getPos(), 5.0f, 5.0f, test))
        {
            choice = 2;
            std::cout << "Is in range C" << std::endl;
        }

        if (choice != 3)
        {
            if (choices[choice] == components[2])
            {
                this->SpawnDoor(this->GetPosition());
                std::cout << "Correct choice!" << std::endl;
            }
            else
            {
                std::cout << "Wrong choice! You suck! >:(" << std::endl;
            }
        }
    }
    else
    {
        this->InteractPortal(playerPos, forwardVec);
    }
}

void MathPuzzle::InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm, vec3 position)
{
    this->SetPosition(position);
    int typeOfQuestion = (int)rand() % 4 + 1;

    //Question is addition or subtraction
    if (typeOfQuestion <= 2)
    {
        components[0] = (int)rand() % maxValuePlusMinus + 1;
        components[1] = (int)rand() % maxValuePlusMinus + 1;
        switch (typeOfQuestion)
        {
        case 1:
        {
            arithmetic = '+';
            components[2] = components[0] + components[1];
            break;
        }
        case 2:
        {
            arithmetic = '-';
            components[2] = components[0] - components[1];
            break;
        }
        }
    }
    else
    {
        switch (typeOfQuestion)
        {
        case 3:
        {
            components[0] = (int)rand() % maxValueMultiplication + 1;
            components[1] = (int)rand() % maxValueMultiplication + 1;
            arithmetic = '*';
            components[2] = components[0] * components[1];
            break;
        }
        case 4:
        {
            components[0] = (int)rand() % maxValueDivision[0] + maxValueMultiplication; //maxValueMultiplication because I wanted the value 10 since it wouldn't stop complaining about magic number.
            components[1] = (int)rand() % maxValueDivision[1] + 2;
            arithmetic = '/';
            components[2] = components[0] / components[1];
            if (components[2] * components[1] + components[1] / 2 < components[0])
            {
                components[2] += 1;
            }
            break;
        }
        }
    }

    int answerRandomizer = (int)rand() % 3 + 1;

    switch (answerRandomizer)
    {
    case 1: //Both other answers higher.
    {
        choices[0] = components[2];
        choices[1] = choices[0] + 1;
        choices[2] = choices[0] + 2;
        break;
    }
    case 2: //Both other answers lower.
    {
        choices[0] = components[2] - 2;
        choices[1] = components[2] - 1;
        choices[2] = components[2];
        if (arithmetic == '/' && choices[1] <= 1)
        {
            choices[0] = components[2];
            choices[1] = choices[0] + 1;
            choices[2] = choices[0] + 2;
        }
        break;
    }
    case 3: //One higher, one lower.
    {
        choices[0] = components[2] - 1;
        choices[1] = components[2];
        choices[2] = components[2] + 1;
        if (arithmetic == '/' && choices[0] <= 0)
        {
            choices[0] = components[2];
            choices[1] = choices[0] + 1;
            choices[2] = choices[0] + 2;
        }
        break;
    }
    }

    puzzlePlatform = new GameObject(rm->get_Models("BasePlatform.obj", gfx), gfx, position, vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));

    float x, y, z, x2, z2, x3, z3;
    y = puzzlePlatform->getWidthHeightDepth().y;

    for (int i = 0; i < 3; i++)
    {
        if (i == 0)
        {
            x = (float)(rand() % (int)puzzlePlatform->getWidthHeightDepth().x - 2);
            z = (float)(rand() % (int)puzzlePlatform->getWidthHeightDepth().z - 2);
        }
        else if (i == 1)
        {
            x2 = (float)(rand() % (int)puzzlePlatform->getWidthHeightDepth().x - 2);
            z2 = (float)(rand() % (int)puzzlePlatform->getWidthHeightDepth().z - 2);

            if (abs(x2 - x) < 5.0f || abs(z2 - z) < 5.0f)
            {
                i--;
            }
        }
        else
        {
            x3 = (float)(rand() % (int)puzzlePlatform->getWidthHeightDepth().x - 2);
            z3 = (float)(rand() % (int)puzzlePlatform->getWidthHeightDepth().z - 2);

            if (abs(x3 - x) < 5.0f || abs(z3 - z) < 5.0f || abs(x3 - x2) < 5.0f || abs(z3 - z2) < 5.0f)
            {
                i--;
            }
        }
    }

    if (x > (puzzlePlatform->getWidthHeightDepth().x / 2.0f))
    {
        x = x - (puzzlePlatform->getWidthHeightDepth().x - 2.0f);
    }
    if (x2 > (puzzlePlatform->getWidthHeightDepth().x / 2.0f))
    {
        x2 = x2 - (puzzlePlatform->getWidthHeightDepth().x - 2.0f);
    }
    if (x3 > (puzzlePlatform->getWidthHeightDepth().x / 2.0f))
    {
        x3 = x3 - (puzzlePlatform->getWidthHeightDepth().x - 2.0f);
    }

    if (z > (puzzlePlatform->getWidthHeightDepth().z / 2.0f))
    {
        z = z - (puzzlePlatform->getWidthHeightDepth().z - 2.0f);
    }
    if (z2 > (puzzlePlatform->getWidthHeightDepth().z / 2.0f))
    {
        z2 = z2 - (puzzlePlatform->getWidthHeightDepth().z - 2.0f);
    }
    if (z3 > (puzzlePlatform->getWidthHeightDepth().z / 2.0f))
    {
        z3 = z3 - (puzzlePlatform->getWidthHeightDepth().z - 2.0f);
    }

    std::cout << puzzlePlatform->getWidthHeightDepth().x << " " << puzzlePlatform->getWidthHeightDepth().y << " " << puzzlePlatform->getWidthHeightDepth().z << std::endl;


    puzzleObjects.push_back(new GameObject(rm->get_Models("BasePlatform.obj", gfx), gfx, vec3(puzzlePlatform->getxPos() + x, puzzlePlatform->getyPos() + y, puzzlePlatform->getzPos() + z), vec3(0.0f, 0.0f, 0.0f), vec3(0.05f, 0.3f, 0.05f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("BasePlatform.obj", gfx), gfx, vec3(puzzlePlatform->getxPos() + x2, puzzlePlatform->getyPos() + y, puzzlePlatform->getzPos() + z2), vec3(0.0f, 0.0f, 0.0f), vec3(0.05f, 0.3f, 0.05f)));
    puzzleObjects.push_back(new GameObject(rm->get_Models("BasePlatform.obj", gfx), gfx, vec3(puzzlePlatform->getxPos() + x3, puzzlePlatform->getyPos() + y, puzzlePlatform->getzPos() + z3), vec3(0.0f, 0.0f, 0.0f), vec3(0.05f, 0.3f, 0.05f)));

    std::cout << this->GetComponents() << std::endl;
}

void MathPuzzle::Update(Graphics*& gfx)
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
