#include "MyGraphic.h"

int windowWidth = 1280;
int windowHeight = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	// Setup our application window
	Window myWindow;


	myWindow.Initialize(hInstance, "Hello Shapes", windowWidth, windowHeight);

	MyGraphic myGraphic;
	myGraphic.Initialize(&myWindow, windowWidth, windowHeight);
	

	

	bool done = false;

	while (!done)
	{

		done = myWindow.ProcessMessage();
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			done = true;
		}
		else if (GetAsyncKeyState(VK_NUMPAD1))
		{
			
		}
		else if (GetAsyncKeyState(VK_NUMPAD2))
		{
			
		}
		else if (GetAsyncKeyState(VK_NUMPAD3))
		{
			
		}

		myGraphic.Update();
		myGraphic.Render();
	}
	
	myGraphic.Terminate();
	return 0;
}