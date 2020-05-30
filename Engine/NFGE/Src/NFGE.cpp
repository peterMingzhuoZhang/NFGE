#include "Precompiled.h"
#include "NFGE.h"
#include "App.h"

NFGE::App NFGE::sApp;

void NFGEApp::ChangeState(std::string name)
{
	NFGE::sApp.ChangeState(std::move(name));
}

void NFGEApp::Run(NFGE::AppConfig appConfig)
{
	NFGE::sApp.Run(std::move(appConfig));
}

void NFGEApp::ShutDown()
{
	PostQuitMessage(0);
}