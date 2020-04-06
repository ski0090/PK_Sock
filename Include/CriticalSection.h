#pragma once
class SOCK_DLL PK_CriticalSection
{
public:
	PK_CriticalSection();
	~PK_CriticalSection();

	void Lock();
	void Unlock();
private:
	CRITICAL_SECTION m_critSec;
};

class SOCK_DLL CriticalSectionLock
{
public:
	CriticalSectionLock(PK_CriticalSection& _criSec);
	~CriticalSectionLock();
private:
	PK_CriticalSection* mCritical;
};