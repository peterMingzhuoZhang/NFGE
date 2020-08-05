#ifndef  INCLUDED_NFGE_GRAPHICS_TEXTURE_H
#define INCLUDED_NFGE_GRAPHICS_TEXTURE_H

namespace NFGE
{
namespace Graphics 
{
class Texture
{
public:
	Texture() = default;
	~Texture();

	bool Initialize(const std::filesystem::path& fileName);
	void Terminate();

	void BindVS(uint32_t slot = 0) const;
	void BindPS(uint32_t slot = 0) const;

	uint32_t GetWidth() const { return mWidth; }
	uint32_t GetHeight() const { return mHeight; }

private:
	friend ID3D11ShaderResourceView* GetShaderResourceView(const Texture & texture);

	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	uint32_t mWidth;
	uint32_t mHeight;
};

} // namespace NFGE
} // namespace Graphics

#endif // ! INCLUDED_NFGE_GRAPHICS_TEXTURE_H
