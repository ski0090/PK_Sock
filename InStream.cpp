#include "PK_Sock.h"


void CInStream::Serialize(void* ioData, uint32_t inByteCount)
{
	Read(ioData, inByteCount);
}

bool CInStream::IsInput() const
{
	return true;
}

void CInStream::Read(void* outData, uint32_t inByteCount)
{
	uint32_t resultHead = mHead + inByteCount;
	if (resultHead > m_strBuffer.length())
	{
		StringUtils::Log("InStream Is OverFrlow");
	}

	//std::memcpy(outData, m_strBuffer.c_str() + mHead, inByteCount);
	m_strBuffer += (char*)outData;
	mHead = resultHead;
}
