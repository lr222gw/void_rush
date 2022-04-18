#include "generation/proto.hpp"
#include "physics/proto.hpp"
#include <cmath>
#include <corecrt_math.h>
#include <fstream>
#include <iostream>
#include <time.h>

#include "Game.h"
#include "debug.h"
#include "Random.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//enable console
#ifdef _DEBUG
	RedirectIOToConsole();
#endif
	Game game(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	game.run();
	//physics_proto();
	
    //physics_proto ();
    //generation_proto ();    
    return 0;
}