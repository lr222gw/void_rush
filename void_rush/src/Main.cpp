#include "generation/proto.hpp"
#include "physics/proto.hpp"
#include "interact/interact.hpp"
#include <cmath>
#include <corecrt_math.h>
#include <fstream>
#include <iostream>
#include <time.h>

#include "3Dproj/App.h"
#include "3Dproj/debug.h"
#include "3Dproj/Random.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//enable console
#ifdef _DEBUG
	RedirectIOToConsole();
#endif
	App game(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	game.run();
    
    return 0;
}