#pragma once
#include "PTGeometry_Shell.h"

class PTCone : public PTGemotry_Shell
{
public:
	void Load(const wchar_t* vertexShader, const wchar_t* pixelShader)
	{
		mMesh = NFGE::Graphics::MeshBuilder::CreateCone(8, 16, 0.5f, 1.0f);
		PTGemotry_Shell::BaseLoad(vertexShader, pixelShader);
	}
	void Update(float deltaTime)
	{

	}
	void Render(const NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Graphics::Camera& camera, const NFGE::Graphics::Color& diffuseColor = NFGE::Graphics::Colors::Gray, const NFGE::Graphics::Color& ambientColor = NFGE::Graphics::Colors::DarkGray)
	{
		PTGemotry_Shell::Render(toWorldMatrix, camera, diffuseColor, ambientColor);
	}
	void UnLoad()
	{
		PTGemotry_Shell::BaseUnLoad();
	}
private:
};