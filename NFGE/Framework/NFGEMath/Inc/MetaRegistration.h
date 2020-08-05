#ifndef INCLUDE_NFGE_MATH_METAREGISTRATION_H
#define INCLUDE_NFGE_MATH_METAREGISTRATION_H

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix4.h"

// Primitive Type Declarations
META_TYPE_DECLARE(NFGE::Math::Vector2)
META_TYPE_DECLARE(NFGE::Math::Vector3)
META_TYPE_DECLARE(NFGE::Math::Vector4)
META_TYPE_DECLARE(NFGE::Math::EditorQuaternion)
META_TYPE_DECLARE(NFGE::Math::Matrix4)

namespace NFGE::Math
{
	void StaticMetaRegister();
}

#endif // !INCLUDE_NFGE_CORE_METAREGISTRATION_H
