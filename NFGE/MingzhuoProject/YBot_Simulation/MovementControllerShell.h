#pragma once


#include "NFGE/Inc/NFGE.h"

class YBot;

class MovementControllerShell
{
public:
	virtual void Load() = 0;
	virtual void Set() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void DebugUI() = 0;
};
