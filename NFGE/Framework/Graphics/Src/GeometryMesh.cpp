#include "Precompiled.h"
#include "GeometryMesh.h"

#include "EffectManager.h"
#include "Light.h"

using namespace NFGE::Graphics;

void NFGE::Graphics::GeometryMesh::Load(DirectionalLight * light)
{
	mMeshBuffer.Initialize(mMesh);
	mMeshBuffer.SetTopology();
	mContext.light = light;
}

void NFGE::Graphics::GeometryMesh::UnLoad()
{
	mMeshBuffer.Terminate();
}

void NFGE::Graphics::GeometryMesh::OldRender(const NFGE::Math::Matrix4 & toWorldMatrix, const NFGE::Graphics::Camera & camera, const NFGE::Graphics::Color & diffuseColor, const NFGE::Graphics::Color & ambientColor)
{
	mContext.custumToWorldMatrix = toWorldMatrix;

	mContext.material.diffuse = diffuseColor;
	mContext.material.ambient = ambientColor;

	Effect* effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::StandardMesh);

	mMeshBuffer.Render(effect, &mContext, camera);
}

void NFGE::Graphics::GeometryMesh::Render(const NFGE::Math::Matrix4 & toWorldMatrix, const NFGE::Graphics::Camera & camera, const NFGE::Graphics::Color & diffuseColor, const NFGE::Graphics::Color & ambientColor)
{
	mContext.custumToWorldMatrix = toWorldMatrix;

	mContext.material.diffuse = diffuseColor;
	mContext.material.ambient = ambientColor;

	auto effectManager = EffectManager::Get();
	Effect* effect = NFGE::Graphics::EffectManager::Get()->GerEffect(NFGE::Graphics::EffectType::StandardMesh);

	effectManager->RegisterRenderObject(effect, &mContext, &mMeshBuffer, camera);
}

void NFGE::Graphics::GeometryMesh::UpdateAdjustMatrix(NFGE::Math::Matrix4 & mat)
{
	mContext.custumAdjustMatrix = std::move(mat);
}
