#ifndef INCLUDED_HELLOYBOT_PTSPHERE_H
#define INCLUDED_HELLOYBOT_PTSPHERE_H

#include "PTGeometry_Shell.h"

class PTSphere : public PTGemotry_Shell
{
public:
	void Load(const wchar_t* vertexShader, const wchar_t* pixelShader)
	{
		mMesh = NFGE::Graphics::MeshBuilder::CreateSphere(8,8,0.5f);
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

#endif // !INCLUDED_HELLOYBOT_PTSPHERE_H

