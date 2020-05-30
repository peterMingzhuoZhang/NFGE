#ifndef INCLUDED_NFGE_GRAPHICS_CONSTANTBUFFER_H
#define INCLUDED_NFGE_GRAPHICS_CONSTANTBUFFER_H

#include "GraphicsSystem.h"

namespace NFGE {
namespace Graphics {

class ConstantBuffer
{
public:
	ConstantBuffer() = default;
	~ConstantBuffer();

	void Initialize(uint32_t constantSize);
	void Terminate();

	void Load(const void * data) const;
	void BindVS(uint32_t sloat = 0) const;
	void BindGS(uint32_t sloat = 0) const;
	void BindPS(uint32_t sloat = 0) const;
	void BindCS(uint32_t sloat = 0) const;

private:
	ID3D11Buffer* mConstantBuffer{ nullptr };

};

template<class T>
class TypedConstantBuffer : public ConstantBuffer
{
public:
	using DataType = T;

	void Initialize()
	{
		static_assert(sizeof(DataType) % 16 == 0, "[TypedConstantBuffer] data type can not divide by 16");
		ConstantBuffer::Initialize(sizeof(DataType));
	}

	void Initialize(const DataType& data)
	{
		ConstantBuffer::Initialize(sizeof(DataType), &data);
	}

	void Load(const DataType& data) const
	{
		ConstantBuffer::Load(&data);
	}
};

}
}

#endif // !INCLUDED_NFGE_GRAPHICS_CONSTANTBUFFER_H
