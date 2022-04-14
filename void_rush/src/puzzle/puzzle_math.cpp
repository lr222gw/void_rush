#include "puzzle_math.hpp"
#include <string>

MathPuzzle::MathPuzzle(const Vector3& position, int seed, int width, int length) : Puzzle(position, seed, width, length)
{
}

std::string MathPuzzle::GetComponents() const
{
    return std::to_string(components[0]) + " " + arithmetic + " " + std::to_string(components[1]) + " = " + std::to_string(components[2]) + "\nThese are your choices: " + std::to_string(choices[0]) + " " + std::to_string(choices[1]) + " " + std::to_string(choices[2]);
}

void MathPuzzle::Interaction(int choice)
{
    if (choices[choice] == components[2])
    {
        //Puzzle::SpawnDoor();
        std::cout << "Correct choice!" << std::endl;
    }
    else
    {
        std::cout << "Wrong choice! You suck! >:(" << std::endl;
    }
}

void MathPuzzle::InitiatePuzzle()
{
    int typeOfQuestion = (int)rand() % 4 + 1;

    //Question is addition or subtraction
    if(typeOfQuestion <= 2)
    {
        components[0] = (int)rand() % maxValuePlusMinus + 1;
        components[1] = (int)rand() % maxValuePlusMinus + 1;
        switch(typeOfQuestion)
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
        switch(typeOfQuestion)
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
                if(components[2] * components[1] + components[1]/2 < components[0])
                {
                    components[2] += 1;
                }
                break;
            }
        }
    }

    int answerRandomizer = (int)rand() % 3 + 1;

    switch(answerRandomizer)
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
            if(arithmetic == '/' && choices[1] <= 1)
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
            if(arithmetic == '/' && choices[0] <= 0)
            {
                choices[0] = components[2];
                choices[1] = choices[0] + 1;
                choices[2] = choices[0] + 2;
            }
            break;
        }
    }
}