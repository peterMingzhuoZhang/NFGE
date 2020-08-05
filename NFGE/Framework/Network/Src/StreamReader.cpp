#include "Precompiled.h"
#include "StreamReader.h"
#include "MemoryStream.h"

using namespace Network;

Network::StreamReader::StreamReader(MemoryStream & memoryStream)
	:mMemoryStream(memoryStream)
{
}

void Network::StreamReader::Read(void * data, uint32_t size)
{
	ASSERT(((mMemoryStream.mHead + size) <= mMemoryStream.mCapacity), "[StreamReader] read out of bound");
	
	memcpy(data,mMemoryStream.mBuffer + mMemoryStream.mHead, size);
	mMemoryStream.mHead += size;
}

void Network::StreamReader::Read(std::string & str)
{
	int size = 0;
	Read(size);

	std::unique_ptr<char> str_c(new char[size]);
	Read(str_c.get(), sizeof(std::string::value_type) * size);
	str.assign(str_c.get(),size);

}
