#include "EditState.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	NFGE::AppConfig config;
	config.appName = "NFGE Editor";
	config.maximize = true;
	config.imGuiDocking = true;
	config.isEditor = true;

	NFGEApp::AddState<EditState>("EditState");
	NFGEApp::Run(config);
	return 0;
}