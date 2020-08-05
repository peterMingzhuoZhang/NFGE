#include "GameState.h"
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	srand(0);
	NFGEApp::AddState<GameState>("GameState_0");
	NFGEApp::Run({});
	return 0;
}
