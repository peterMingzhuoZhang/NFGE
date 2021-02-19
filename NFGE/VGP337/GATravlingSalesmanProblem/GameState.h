#pragma once

#include "NFGE/Inc/NFGE.h"

class GameState : public NFGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
private:
	float mdeltaTime;

};


