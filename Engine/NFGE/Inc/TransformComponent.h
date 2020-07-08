#ifndef INCLUDE_NFGE_TRANSFORMCOMPONENT_H
#define INCLUDE_NFGE_TRANSFORMCOMPONENT_H

#include "Component.h"

namespace NFGE
{
	class TransformComponent : public Component
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void Render() override;
		void InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass*, uint8_t*)) override;

		Math::Vector3 position;
		Math::EditorQuaternion rotation;
		Math::Vector3 scale;

		Math::Vector3 finalPosition;
		Math::Quaternion finalRotation;
		Math::Vector3 fianlScale;
	};
}

#endif // !INCLUDE_NFGE_TRANSFORMCOMPONENT_H
