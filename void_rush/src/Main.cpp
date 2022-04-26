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
	//Following junk is to recieve arguments from app executioner
	LPWSTR* szArglist;
	int nArgs;	
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);	
#endif
	
	//Declare game
	App game(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	GameStatesEnum initialGameState = GameStatesEnum::TO_MENU;
#ifdef _DEBUG
	//Decide if menu is to be skipped
	const std::wstring skipMenu = L"skipMenu";
	if (nArgs > 1 && szArglist[1] == skipMenu) {
		initialGameState = GameStatesEnum::TO_GAME;		
	}
#endif

	game.set_initial_gamestate(initialGameState);
	//Run game
	game.run();
    
    return 0;
}