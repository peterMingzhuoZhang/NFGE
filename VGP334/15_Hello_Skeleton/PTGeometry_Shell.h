#ifndef INCLUDED_HELLOSKELETON_PTGEOMETRY_SHELL_H
#define INCLUDED_HELLOSKELETON_PTGEOMETRY_SHELL_H

#define _CRT_SECURE_NO_WARNINGS
#include <NFGE/Inc/NFGE.h>

// Using selfDefine type --------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

using namespace NFGE::Graphics;

class PTGemotry_Shell
{
protected:
	NFGE::Graphics::Mesh mMesh;
	NFGE::Graphics::MeshBuffer mMeshBuffer;
	NFGE::Graphics::Effects::StandardMeshEffect::StandardMeshEffectContext mContext;

protected:
	void BaseLoad(DirectionalLight* light)
	{
		mMeshBuffer.Initialize(mMesh);
		mMeshBuffer.SetTopology();
		mContext.light = light;
	}
	void BaseUnLoad()
	{
		mMeshBuffer.Terminate();
	}
	void Render(NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Graphics::Camera& camera, const NFGE::Graphics::Color& diffuseColor = NFGE::Graphics::Colors::Gray, const NFGE::Graphics::Color& ambientColor = NFGE::Graphics::Colors::DarkGray)
	{
		NFGE::Graphics::RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();

		mContext.custumToWorldMatrix = toWorldMatrix;

		mContext.material.diffuse = diffuseColor;
		mContext.material.ambient = ambientColor;
		
		Effect* effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::StandardMesh);

		mMeshBuffer.Render(effect, &mContext, camera);
	}
public:
	void UpdateAdjustMatrix(NFGE::Math::Matrix4& mat)
	{
		mContext.custumAdjustMatrix = std::move(mat);
	}
};

#endif // !INCLUDED_HELLOSKELETON_PTGEOMETRY_SHELL_H

