#include "protoPuzzle.hpp"
#include <malloc.h>

ProtoPuzzle::ProtoPuzzle(Graphics*& gfx, ResourceManager*& rm, CollisionHandler& colHandler, SoundManager* soundManager) : gfxPuzzle(gfx), rmPuzzle(rm)
{
    seed = (int)time(0);
    srand(seed);

    this->math = new MathPuzzle(seed, gfx, rm, colHandler, soundManager);
    this->hidden = new HiddenPuzzle(seed, gfx, rm, colHandler, soundManager);
    this->memory = new MemoryPuzzle(seed, gfx, rm, colHandler, soundManager);
    this->move = new MovePuzzle(seed, gfx, rm, colHandler, soundManager);

    this->puzzleList.push_back(math);
    this->puzzleList.push_back(hidden);
    this->puzzleList.push_back(memory);
    this->puzzleList.push_back(move);
}

int ProtoPuzzle::ChoosePuzzle()
{
    return (int)rand() % this->PUZZLES;
}

ProtoPuzzle::~ProtoPuzzle()
{
    delete this->math;
    delete this->hidden;
    delete this->memory;
    delete this->move;
}

void ProtoPuzzle::Interact(vec3 playerPos, vec3 forwardVec)
{
    this->puzzleList[chosenPuzzle]->Interaction(playerPos, forwardVec);
}

void ProtoPuzzle::UpdatePlayerPosition(vec3 Pos)
{
    math->UpdatePlayerPos(Pos);
    memory->UpdatePlayerPos(Pos);
}

void ProtoPuzzle::Initiate(vec3 platformPosition)
{
    if (chosenPuzzle != -1)
    {
        this->puzzleList[chosenPuzzle]->ResetState();
    }
    chosenPuzzle = ChoosePuzzle();
    this->puzzleList[chosenPuzzle]->InitiatePuzzle(this->gfxPuzzle, this->rmPuzzle, platformPosition);
}

void ProtoPuzzle::Update()
{
    this->puzzleList[chosenPuzzle]->Update(gfxPuzzle);
}

bool ProtoPuzzle::isCompleted()
{
    return this->puzzleList[chosenPuzzle]->GetPortalCompleted();
}
