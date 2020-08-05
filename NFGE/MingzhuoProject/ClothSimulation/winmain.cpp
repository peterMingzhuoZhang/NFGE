#include "GameState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	
	NFGEApp::AddState<GameState>("GameState");
	NFGEApp::Run({ "Grass Simulation"});
	return 0;
}