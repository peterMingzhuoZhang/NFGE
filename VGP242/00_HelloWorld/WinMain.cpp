#include <Core\Inc\Core.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	LOG("Hello %d", 1 + 2);
	ASSERT(7 == 42, "7 does not equal to 42");
	return 0;
}