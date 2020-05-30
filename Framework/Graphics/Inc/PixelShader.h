#ifndef INCLUDED_NFGE_GRAPHICS_PIXELSHADER_H
#define INCLUDED_NFGE_GRAPHICS_PIXELSHADER_H

namespace NFGE {
namespace Graphics {

class PixelShader
{
public:
	void Initialize(const wchar_t* fileName, const char* shaderName = "PS");
	void Terminate();

	void Bind();
private:
	ID3D11PixelShader* pixelshader{ nullptr };
};

} // namespace Graphics
} // namespace NFGE

#endif // !INCLUDED_NFGE_GRAPHICS_PIXELSHADER_H
