#include "GameState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	NFGEApp::AddState<GameState>("GameState_0");
	NFGEApp::Run({ "Hello Post Processing Advance" });
	return 0;
}