#include "protoPuzzle.hpp"
#include <malloc.h>

ProtoPuzzle::ProtoPuzzle(Graphics*& gfx, ResourceManager*& rm) : gfxPuzzle(gfx), rmPuzzle(rm)
{
    seed = (int)time(0);
    srand(seed);

    this->math = new MathPuzzle(Vector3(1, 1, 1), seed, 1, 1);
    this->hidden = new HiddenPuzzle(Vector3(1, 1, 1), seed, 1, 1);

    this->puzzleList.push_back(math);
    this->puzzleList.push_back(hidden);
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

void ProtoPuzzle::Interact(vec3 playerPos)
{
    this->puzzleList[chosenPuzzle]->Interaction(playerPos);
}

void ProtoPuzzle::Initiate()
{
    chosenPuzzle = ChoosePuzzle();
    this->puzzleList[chosenPuzzle]->InitiatePuzzle(this->gfxPuzzle, this->rmPuzzle);
    if (chosenPuzzle == 0)
    {
        std::cout << this->math->GetComponents() << std::endl;
    }
}

void ProtoPuzzle::Update()
{
    this->puzzleList[chosenPuzzle]->Update(gfxPuzzle);
}
