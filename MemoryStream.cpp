#include "PK_Sock.h"

CMemoryStream::CMemoryStream() :m_pLinking(nullptr),
m_bIsLea(FALSE), mHead(0)
{
}

CMemoryStream::CMemoryStream(char* inBuffer)
:m_pLinking(nullptr),
m_strBuffer(inBuffer), m_bIsLea(FALSE), mHead(0)
{
};