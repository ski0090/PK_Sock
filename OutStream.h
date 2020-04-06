#pragma once

class SOCK_DLL COutStream :
	public CMemoryStream
{
public:
	COutStream()
		:CMemoryStream()
	{
	};
	~COutStream(){
	};

	virtual void Serialize(void* ioData, uint32_t inByteCount) override;

	virtual bool IsInput() const override;
	const char* GetBufferPtr() const { 
		if (m_bIsLea)
			return m_strLea_Buffer.c_str();

		return m_strBuffer.c_str();
	}
	uint32_t	GetLength() const { 
		return mHead;
	}
private:
	void	Write(const void* inData,
		size_t inByteCount);

	
};

