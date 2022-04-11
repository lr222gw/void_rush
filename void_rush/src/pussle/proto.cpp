#include "proto.hpp"
#include <malloc.h>
#include <vector>

const int PUZZLES = 2;

void pussle_proto ()
{
    int seed = time(0);
    srand(seed);
    int chosenPuzzle = ChoosePuzzle();

    MathPuzzle math(Vector3(1, 1, 1), seed);
    HiddenPuzzle hidden(Vector3(1,1,1), seed);

    
    std::vector<Puzzle*> puzzleList;
    puzzleList.push_back(&math);
    puzzleList.push_back(&hidden);

    puzzleList[chosenPuzzle]->InitiatePuzzle();
    if(chosenPuzzle == 0)
    {
        std::cout << math.GetComponents() << std::endl;
    }
}

int ChoosePuzzle()
{
    return (int)rand() % PUZZLES ;
}