#ifndef INCLUDE_NETWORK_MEMORYSTREAM_H
#define INCLUDE_NETWORK_MEMORYSTREAM_H

namespace Network
{
class MemoryStream
{
public:
	MemoryStream() = default;
	MemoryStream(uint32_t capacity);
	MemoryStream(const uint8_t* data, uint32_t size);
	~MemoryStream();

	void Rewind();

	const uint8_t* GetData() const	{ return mBuffer; }
	uint32_t GetSize() const { return mHead; }
	bool IsEmpty() const { return mHead == mCapacity; }

private:
	MemoryStream(const MemoryStream&) = delete;
	MemoryStream& operator=(const MemoryStream&) = delete;

	friend class StreamWriter;
	friend class StreamReader;

	void ReallocBuffer(uint32_t newCapacity);

	uint8_t* mBuffer{ nullptr };
	uint32_t mHead{ 0 };
	uint32_t mCapacity{ 0 };
};

} // namespace Network

#endif // !INCLUDE_NETWORK_MEMORYSTREAM_H
