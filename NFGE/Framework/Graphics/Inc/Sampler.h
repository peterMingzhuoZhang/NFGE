#ifndef INCLUDED_NFGE_GRAPHICS_SAMPLER_H
#define INCLUDED_NFGE_GRAPHICS_SAMPLER_H

namespace NFGE
{
namespace Graphics
{
class Sampler
{
public:
	enum class Filter { Point, Linear, Anisotropic };
	enum class AddressMode { Border, Clamp, Mirror, Wrap };

	Sampler() = default;
	~Sampler();

	void Initialize(Filter filter, AddressMode addressMode);
	void Terminate();

	void BindPS(uint32_t sloat = 0) const;
	void BindVS(uint32_t sloat = 0) const;
private:
	ID3D11SamplerState* mSampler;
};

} // namespace NFGE
} // namespace Graphics


#endif // !INCLUDED_NFGE_GRAPHICS_SAMPLER_H
