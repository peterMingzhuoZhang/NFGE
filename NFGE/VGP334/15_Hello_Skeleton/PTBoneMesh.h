#ifndef INCLUDED_HELLOSKELETON_BONEMESH_H
#define INCLUDED_HELLOSKELETON_BONEMESH_H

#include "PTCone.h"
#include "PTSphere.h"

using namespace NFGE::Math;

class PTBoneMesh
{
public:
	void Load(const NFGE::Math::Vector4& ambientColor, const NFGE::Math::Vector4& diffuseColor, DirectionalLight* light);
	void Update(float deltaTime){}
	void Render(const NFGE::Math::Matrix4& toParentMat, NFGE::Math::Matrix4& toWorldMat, const NFGE::Graphics::Camera& camera, bool isSelected);
	void DebugUI();
	void UnLoad();

	// Setter & Getter
	void SetDiffuseColor(const NFGE::Math::Vector4& color) { mDiffuseColor = color; }
	NFGE::Math::Vector4 GetDiffuseColor() const { return mDiffuseColor; }

	void SetAmbientColor(const NFGE::Math::Vector4& color) { mAmbientColor = color; }
	NFGE::Math::Vector4 GetAmbientColor() const { return mAmbientColor; }

private:
	PTCone mConeGeometry;
	PTSphere mSphereGeometry;

	NFGE::Math::Vector4		mAmbientColor;
	NFGE::Math::Vector4		mDiffuseColor;

};

#endif // !INCLUDED_HELLOSKELETON_BONEMESH_H
