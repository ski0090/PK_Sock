#include "PK_Sock.h"

void COutStream::Serialize(void* ioData, uint32_t inByteCount)
{
	Write(ioData,inByteCount);
}

bool COutStream::IsInput() const
{
	return false;
}


void COutStream::Write(const void* inData,
	size_t inByteCount)
{
	//make sure we have space...
	uint32_t resultHead = mHead + static_cast<uint32_t>(inByteCount);
	//copy into buffer at head
	//std::memcpy((char*)m_strBuffer.c_str() + mHead, inData, inByteCount);
	m_strBuffer += (char*)inData;
	//increment head for next write
	mHead = resultHead;
}
