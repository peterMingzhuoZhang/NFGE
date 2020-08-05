#include "GameState.h"
#include <ImGui/Inc/imgui.h>

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	NFGEApp::AddState<BuildPolyGraphState>("GameState_0");
	NFGEApp::AddState<MainGameState>("GameState_1");
	NFGEApp::Run({});
	return 0;
}