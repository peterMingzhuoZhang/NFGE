#include "GameState.h"


int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	NFGEApp::AddState<GameState>("GameState_0");
	NFGEApp::Run({});
	return 0;
}