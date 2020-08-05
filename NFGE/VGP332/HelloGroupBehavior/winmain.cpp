#include "GameState.h"

std::vector<NFGE::Graphics::TextureId> TriangleDude::mAllTextures;
AtlasInfo TriangleDude::mAltasInfo;

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	NFGEApp::AddState<GameState>("GameState_0");
	NFGEApp::Run({});
	return 0;
}