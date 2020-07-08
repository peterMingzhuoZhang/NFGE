#ifndef INCLUDE_NFGE_GRAPHICS_METAREGISTRATION_H
#define INCLUDE_NFGE_GRAPHICS_METAREGISTRATION_H

#include "MeshBuffer.h"

// Primitive Type Declarations
META_TYPE_DECLARE(NFGE::Graphics::MeshBuffer::Topology)

namespace NFGE::Graphics
{
	void StaticMetaRegister();
}

#endif // !INCLUDE_NFGE_GRAPHICS_METAREGISTRATION_H
