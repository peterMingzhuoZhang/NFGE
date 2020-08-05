#ifndef INCLUDED_NFGE_GRAPHICS_MATERIAL_H
#define INCLUDED_NFGE_GRAPHICS_MATERIAL_H

#include "Colors.h"

namespace NFGE::Graphics
{
	struct Material
	{
		Color ambient;
		Color diffuse;
		Color specular;
		float power;
		float padding[3];

		void SetAlpha(float alpha) { ambient.w = alpha; diffuse.w = alpha; specular.w = alpha;}
	};
} // namespace NFGE::Graphics

#endif // !INCLUDED_NFGE_GRAPHICS_MATERIAL_H
