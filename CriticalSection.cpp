#include "PK_Sock.h"
PK_CriticalSection::PK_CriticalSection()
{
	InitializeCriticalSectionEx(&m_critSec, 0, 0);
}


PK_CriticalSection::~PK_CriticalSection()
{
	DeleteCriticalSection(&m_critSec);
}

void PK_CriticalSection::Lock()
{
	EnterCriticalSection(&m_critSec);
}

void PK_CriticalSection::Unlock()
{
	LeaveCriticalSection(&m_critSec);
}

CriticalSectionLock::CriticalSectionLock(PK_CriticalSection& _criSec)
{
	mCritical = &_criSec;
	mCritical->Lock();
}

CriticalSectionLock::~CriticalSectionLock()
{
	mCritical->Unlock();
}
