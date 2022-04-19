#include "puzzle_hidden.hpp"

HiddenPuzzle::HiddenPuzzle(const Vector3& position, int seed, int width, int length) : Puzzle(position, seed, width, length)
{
    
}

void HiddenPuzzle::Interaction(vec3 playerPos)
{
    //Check if left click is pressed while player is close enough and looking at the hitbox of the key.
    if ((puzzleObjects[0]->getPos() - playerPos).length() < 10.0f)
    {
        std::cout << "Key Picked Up!" << std::endl;
        this->keyPickedUp = true;
    }
}

void HiddenPuzzle::InitiatePuzzle(Graphics*& gfx, ResourceManager*& rm)
{
    this->keyPickedUp = false;
    std::cout << "Hidden puzzle chosen" << std::endl;
    //Based on the size of the platform, place out the key in a valid position where the key can be reached by the player.
    puzzleObjects.push_back(new GameObject(rm->get_Models("Key.obj", gfx), gfx, vec3(0.0f, 25.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)));
}

void HiddenPuzzle::Update(Graphics*& gfx)
{
    if (!this->keyPickedUp)
    {
        for (int i = 0; i < puzzleObjects.size(); i++) {

            puzzleObjects[i]->updateMatrix();
            puzzleObjects[i]->update();
            puzzleObjects[i]->updateVertexShader(gfx);
            puzzleObjects[i]->updatePixelShader(gfx);
            puzzleObjects[i]->draw(gfx);
        }
    }
}
