#include "protoPuzzle.hpp"
#include <malloc.h>

ProtoPuzzle::ProtoPuzzle()
{
    seed = (int)time(0);
    srand(seed);
    int chosenPuzzle = ChoosePuzzle();

    this->math = new MathPuzzle(Vector3(1, 1, 1), seed, 1, 1);
    this->hidden = new HiddenPuzzle(Vector3(1, 1, 1), seed, 1, 1);

    //MathPuzzle math(Vector3(1, 1, 1), seed, 1, 1);
    //HiddenPuzzle hidden(Vector3(1, 1, 1), seed, 1, 1);

    this->puzzleList.push_back(math);
    this->puzzleList.push_back(hidden);

    this->puzzleList[chosenPuzzle]->InitiatePuzzle();
    if (chosenPuzzle == 0)
    {
        std::cout << this->math->GetComponents() << std::endl;
    }
}

int ProtoPuzzle::ChoosePuzzle()
{
    return (int)rand() % this->PUZZLES;
}

ProtoPuzzle::~ProtoPuzzle()
{
    delete this->math;
    delete this->hidden;
}

void ProtoPuzzle::Interact(int choice)
{
    this->puzzleList[chosenPuzzle]->Interaction(choice);
}
