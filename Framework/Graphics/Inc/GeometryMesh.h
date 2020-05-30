#ifndef INCLUDED_NFGE_GRAPHICSGEOMETRYMESH_H
#define INCLUDED_NFGE_GRAPHICSGEOMETRYMESH_H

#include "Mesh.h"
#include "MeshBuffer.h"
#include "Effect.h"

namespace NFGE::Graphics
{
	class GeometryMesh
	{
	protected:
		NFGE::Graphics::Mesh mMesh;
		NFGE::Graphics::MeshBuffer mMeshBuffer;
		NFGE::Graphics::Effects::StandardMeshEffect::StandardMeshEffectContext mContext;

	public:
		void Load(DirectionalLight* light);
		void UnLoad();
		void OldRender(const NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Graphics::Camera& camera, const NFGE::Graphics::Color& diffuseColor = NFGE::Graphics::Colors::Gray, const NFGE::Graphics::Color& ambientColor = NFGE::Graphics::Colors::DarkGray);
		void Render(const NFGE::Math::Matrix4& toWorldMatrix, const NFGE::Graphics::Camera& camera, const NFGE::Graphics::Color& diffuseColor = NFGE::Graphics::Colors::Gray, const NFGE::Graphics::Color& ambientColor = NFGE::Graphics::Colors::DarkGray);
		void UpdateAdjustMatrix(NFGE::Math::Matrix4& mat);
	};
}
#endif // !INCLUDED_HELLOSKELETON_PTGEOMETRY_SHELL_H

