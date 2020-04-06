#pragma once
class SOCK_DLL CMemoryStream abstract
{
public:
	CMemoryStream();
	CMemoryStream(char* inBuffer);
	~CMemoryStream()
	{
	};
	bool IsAccep_LEA(){
		return m_bIsLea;
	}

	virtual void Serialize(void* ioData,
		uint32_t inByteCount) PURE;
	virtual bool IsInput() const PURE;

	template<typename T>
	void Serialize(T& ioData);

protected:
	string	m_strBuffer;
	string	m_strLea_Buffer;
	uint32_t mHead;
	LinkingContext* m_pLinking;
	bool m_bIsLea;
};