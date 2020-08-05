#include "Precompiled.h"
#include "StreamWriter.h"

#include "MemoryStream.h"

using namespace Network;

StreamWriter::StreamWriter(MemoryStream& memoryStream)
	: mMemoryStream( memoryStream)
{

}

void StreamWriter::Write(const void* data, uint32_t size)
{
	if ((mMemoryStream.mHead + size ) >= mMemoryStream.mCapacity )										
	{												
		mMemoryStream.ReallocBuffer(mMemoryStream.mCapacity + size + 1024);
	}
	memcpy(mMemoryStream.mBuffer + mMemoryStream.mHead, data, size);
	mMemoryStream.mHead += size;

}

void StreamWriter::Write(const std::string& str)
{
	int size = static_cast<int>(str.size());
	Write(size);
	Write(str.data(), size * sizeof(std::string::value_type));
}