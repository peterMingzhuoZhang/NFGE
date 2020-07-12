#ifndef INCLUDE_NFGE_TRANSFORMCOMPONENT_H
#define INCLUDE_NFGE_TRANSFORMCOMPONENT_H

#include "Component.h"

namespace NFGE
{
	class GameObject;
	class TransformComponent : public Component
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void Render() override;
		void InspectorUI(void(*ShowMetaClassInInspector)(const NFGE::Core::Meta::MetaClass*, uint8_t*)) override;

		Math::Vector3 position;
		Math::EditorQuaternion rotation;
		Math::Vector3 scale{1.0f,1.0f,1.0f};

		Math::Matrix4 finalTransform;
		Math::Matrix4 finalRotationMatrix;

		void UpdateFinalTransform(GameObject* parent);


	};
}

#endif // !INCLUDE_NFGE_TRANSFORMCOMPONENT_H
