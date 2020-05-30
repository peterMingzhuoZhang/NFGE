#pragma once
#include <NFGE/Inc/NFGE.h>
#include "ScreenTriangle.h"
#include "ColorMesh.h"
#include "TextureMesh.h"
#include "StandardMesh.h"

using namespace NFGE::Core;
using namespace NFGE::Graphics;
using namespace NFGE::Input;
using namespace NFGE::Math;

class GameState : public NFGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	DirectionalLight* mMainLight;

	ScreenTriangle mScreenTriangle;
	ColorMesh mColorMesh;
	TextureMesh mTextureMesh;
	StandardMesh mStandardMesh;
	Model mModelMesh;
};