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

		Math::Vector3 position;
		Math::EditorQuaternion rotation;
	};
}

#endif // !INCLUDE_NFGE_TRANSFORMCOMPONENT_H
