#pragma once

class SOCK_DLL CInStream :
	public CMemoryStream
{
public:
	CInStream(char* inBuffer, uint32_t inByteCount) :
		CMemoryStream(inBuffer)
	{
		mHead = 0;
	};

	virtual void Serialize(void* ioData, uint32_t inByteCount) override;
	virtual bool IsInput() const override;

private:
	void		Read(void* outData, uint32_t inByteCount);

};

