#ifndef INCLUDED_NFGE_GRAPHICS_LIGHT_H
#define INCLUDED_NFGE_GRAPHICS_LIGHT_H

#include "Colors.h"

namespace NFGE::Graphics
{
	struct PointLight
	{
		Math::Vector3 position;
		float padding;
		Color ambient;
		Color diffuse;
		Color specular;
	};

	struct DirectionalLight
	{
		Math::Vector3 direction;
		float padding;
		Color ambient;
		Color diffuse;
		Color specular;
	};
} // namespace NFGE::Graphics

#endif // !INCLUDED_NFGE_GRAPHICS_LIGHT_H
