#ifndef INCLUDE_NFGE_COLLIDERCOMPONENT_H
#define INCLUDE_NFGE_COLLIDERCOMPONENT_H

#include "Component.h"

namespace NFGE
{
	class TransformComponent;
	class ColliderComponent : public Component
	{
	public:
		META_CLASS_DECLARE;
		void Initialize() override;
		void Render() override;
		void InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass*, uint8_t*)) override;

		Math::Vector3 center;
		Math::Vector3 extend;

		const TransformComponent* mTransformComponent = nullptr;
	};
}

#endif // !INCLUDE_NFGE_COLLIDERCOMPONENT_H
