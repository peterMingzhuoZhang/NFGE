#include "Precompiled.h"
#include "ModelIO.h"

bool NFGE::Graphics::ModelIO::IsEndOf(const char* end, const char* string)
{
	int endLength = static_cast<int>(strlen(end));
	int stringLength = static_cast<int>(strlen(string));
	for (int i = 0; i < endLength; i++)
	{
		if (end[endLength - 1 - i] != string[stringLength - 1 - i]) return false;
	}
	return true;
}