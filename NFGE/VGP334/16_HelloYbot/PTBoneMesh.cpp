#include "PTBoneMesh.h"

namespace 
{
	const wchar_t* MeshShaderFileName = L"../../Assets/Shaders/Standard_noTexture.fx";
}

void PTBoneMesh::Load(const NFGE::Math::Vector4& ambientColor, const NFGE::Math::Vector4& diffuseColor)
{
	mAmbientColor = ambientColor;
	mDiffuseColor = diffuseColor;
	mConeGeometry.Load(MeshShaderFileName, MeshShaderFileName);
	mSphereGeometry.Load(MeshShaderFileName, MeshShaderFileName);
}

void PTBoneMesh::Render(const NFGE::Math::Matrix4& toParentMat, const NFGE::Math::Matrix4 & toWorldMat, const NFGE::Graphics::Camera & camera, bool isSelected)
{
	//TODO :: Update cone adjust matrtix
	Vector3 toParent = { toParentMat._41, toParentMat._42, toParentMat._43 };
	float scaleY = Magnitude(toParent) - 2.0f;

	Quaternion qua = QuaternionFromTo({ 0.0f,1.0f,0.0f }, -toParent);

	Vector3 toBottom = { 0.0f,0.5f,0.0f };
	Vector3 toAboveSphere = { 0.0f,1.0f,0.0f };
	Vector3 adjustScale = {2.0f, scaleY, 2.0f};

	NFGE::Math::Matrix4 coneAdjustMat = NFGE::Math::Matrix4::sTranslation(toBottom) * NFGE::Math::Matrix4::sScaling(adjustScale) * NFGE::Math::Matrix4::sTranslation(toAboveSphere) * MatrixRotationQuaternion(qua);

	mConeGeometry.UpdateAdjustMatrix(coneAdjustMat);

	mConeGeometry.Render(toWorldMat, camera, isSelected ? NFGE::Graphics::Colors::White : mDiffuseColor, mAmbientColor);
	mSphereGeometry.Render(toWorldMat, camera, isSelected ? NFGE::Graphics::Colors::White : mDiffuseColor, mAmbientColor);

}

void PTBoneMesh::DebugUI()
{
}

void PTBoneMesh::UnLoad()
{
	mConeGeometry.UnLoad();
	mSphereGeometry.UnLoad();
}
