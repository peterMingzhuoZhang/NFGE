#ifndef INCLUDED_HELLOROBOTICARM_PTCUBE_H
#define INCLUDED_HELLOROBOTICARM_PTCUBE_H

#include "PTGeometry_Shell.h"

class PTCube : public PTGemotry_Shell
{
public:
	void Load(const wchar_t* vertexShader, const wchar_t* pixelShader)
	{
		mMesh = NFGE::Graphics::MeshBuilder::CreateCube();
		PTGemotry_Shell::BaseLoad(vertexShader, pixelShader);
	}
	void Update(float deltaTime)
	{

	}
	void Render(const NFGE::Math::Matrix4& adjestMatrix, const NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Graphics::Camera& camera, const NFGE::Graphics::Color& diffuseColor = NFGE::Graphics::Colors::Gray, const NFGE::Graphics::Color& ambientColor = NFGE::Graphics::Colors::DarkGray)
	{

		mVertexShader.Bind();
		mPixelShader.Bind();

		NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

		NFGE::Math::Matrix4 rotMat = NFGE::Math::MatrixRotationQuaternion(mRotation);
		mMatrices[0].Identity();
		NFGE::Math::Matrix4 scl = NFGE::Math::Matrix4::sScaling(mScale);
		mMatrices[0] = (scl * rotMat * mMatrices[0].Translation(mPosition.x, mPosition.y, mPosition.z) * adjestMatrix * toWorldMatrix);
		mMatrices[1] = (camera.GetViewMatrix());
		mMatrices[2] = (camera.GetPerspectiveMatrix());

		TransformData transformData;
		transformData.world = Transpose(mMatrices[0]);
		transformData.wvp = Transpose(mMatrices[0] * mMatrices[1] * mMatrices[2]);
		transformData.viewPositon = camera.GetPosition();
		mTransformBuffer.Load(transformData);
		mTransformBuffer.BindVS(0);
		mTransformBuffer.BindPS(0);

		mMaterial.diffuse = diffuseColor;
		mMaterial.ambient = ambientColor;
		mMaterialBuffer.Load(mMaterial);
		mMaterialBuffer.BindVS(2);
		mMaterialBuffer.BindPS(2);

		auto rs = NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Solid");
		rs->Set();

		mMeshBuffer.Render();
		rs->Clear();
	}
	void UnLoad()
	{
		PTGemotry_Shell::BaseUnLoad();
	}
private:

};

#endif // !INCLUDED_HELLOROBOTICARM_PTCUBE_H

