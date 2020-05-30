#ifndef INCLUDE_NETWORK_STREAMWRITER_H
#define INCLUDE_NETWORK_STREAMWRITER_H

namespace Network
{
class MemoryStream;

class StreamWriter
{
public:
	StreamWriter(MemoryStream& memoryStream);
	
	void Write(const void* data, uint32_t size);

	template<class T>
	void Write(T data)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Can not write other than primitive data in to memory stream");
		Write(&data, sizeof(T));
	}


	void Write(const std::string& str);
	

private:
	MemoryStream& mMemoryStream;
};

} // namespace Network

#endif // !INCLUDE_NETWORK_STREAMWRITER_H
