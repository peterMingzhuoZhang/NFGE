#ifndef INCLUDED_NFGE_GRAPHICS_VERTEXSHADER_H
#define INCLUDED_NFGE_GRAPHICS_VERTEXSHADER_H

#include "VertexType.h"

namespace NFGE {
namespace Graphics {

class VertexShader
{
public:
	void Initialize(const wchar_t* fileName, uint32_t vertexFormat);
	void Terminate();

	void Bind();
private:
	ID3D11VertexShader* vertexShader { nullptr };
	ID3D11InputLayout* inputLayout{ nullptr };
};

} // namespace Graphics
} // namespace NFGE

#endif // !INCLUDED_NFGE_GRAPHICS_VERTEXSHADER_H
