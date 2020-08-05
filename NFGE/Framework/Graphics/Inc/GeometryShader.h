#ifndef INCLUDED_NFGE_GRAPHICS_GEOMETRYSHADER_H
#define INCLUDED_NFGE_GRAPHICS_GEOMETRYSHADER_H

#include "VertexType.h"

namespace NFGE {
	namespace Graphics {

		class GeometryShader
		{
		public:
			void Initialize(const wchar_t* fileName);
			void Terminate();

			void Bind();
			void UnBind();
		private:
			ID3D11GeometryShader* geometryShader{ nullptr };
		};

	} // namespace Graphics
} // namespace NFGE

#endif // !INCLUDED_NFGE_GRAPHICS_GEOMETRYSHADER_H