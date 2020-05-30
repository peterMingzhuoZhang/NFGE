#ifndef INCLUDE_NETWORK_STREAMREADER_H
#define INCLUDE_NETWORK_STREAMREADER_H

namespace Network
{
class MemoryStream;

class StreamReader
{
public:
	StreamReader(MemoryStream& memoryStream);
	
	template<class T>
	void Read(T& data)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Can not write other than primitive data in to memory stream");
		Read(&data, sizeof(T));
	}
	void Read(std::string& str);

private:
	void Read(void* data, uint32_t size);
	MemoryStream& mMemoryStream;
};

} // namespace Network

#endif // !INCLUDE_NETWORK_STREAMREADER_H
