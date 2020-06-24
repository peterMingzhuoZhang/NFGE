#ifndef INCLUDE_NFGE_MESHRENDERCOMPONENT_H
#define INCLUDE_NFGE_MESHRENDERCOMPONENT_H

#include "Component.h"

namespace NFGE
{
	enum MeshRenderGeometryType
	{
		Sphere = 0,
		Cube,
		Cylinder,
		Cone,
		Toro
	};
	struct MeshRenderContext
	{
		float mRadius;

	};

	class TransformComponent;
	class MeshRenderComponent : public Component
	{
	public:
		META_CLASS_DECLARE;
		void Initialize() override;
		void Render() override;

		NFGE::Graphics::MeshBuffer mMeshBuffer;
		MeshRenderContext mContext;
		NFGE::Graphics::Mesh mMesh;
		MeshRenderGeometryType mGemotryType;


		//Editor control variables

		const TransformComponent* mTransformComponent = nullptr;
	};
}

#endif // !INCLUDE_NFGE_MESHRENDERCOMPONENT_H
