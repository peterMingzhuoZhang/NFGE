#include "GameState.h"
#include "ForZory/Zory_GameState.h"

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	NFGEApp::AddState<GameState>("GameState_0");
	//NFGEApp::AddState<Zory_GameState>("GameState_1");
	NFGEApp::Run({});
	return 0;
}
