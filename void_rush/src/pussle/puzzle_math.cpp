#include "puzzle_math.hpp"

MathPuzzle::MathPuzzle(const Vector3& position, int seed) : Puzzle(position, seed)
{
}

std::string MathPuzzle::GetComponents() const
{
    return std::to_string(components[0]) + " " + arithmetic + " " + std::to_string(components[1]) + " = " + std::to_string(components[2]);
}

void MathPuzzle::Interaction()
{
    //Check if cooldown is gone for selecting an answer

    //Add interaction logic such as if left click is pressed while the player is close enough and looking at a hitbox of one of the answers.

    //If correct answer then:
    Puzzle::SpawnDoor(); 

    //Else punish the player by setting the cooldown they need to wait to x amount of seconds.
}

void MathPuzzle::InitiatePuzzle()
{
    srand(Puzzle::GetSeed());
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