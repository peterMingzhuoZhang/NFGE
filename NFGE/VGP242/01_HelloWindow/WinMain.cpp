#include <Core\Inc\Core.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	NFGE::Core::Window myWindow;
	myWindow.Initialize(hInstance, "Hello Window", 1280, 720);

	bool done = false;

	while (!done)
	{
		done = myWindow.ProcessMessage();

		// run your game logic ...
	}

	myWindow.Terminate();
	return 0;
}