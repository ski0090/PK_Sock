#pragma once
#ifdef _USE_SSL
using namespace std;
class SOCK_DLL SSL_Utill
{

public:
	
	//�ݵ�� ���� �ʱ�ȭ�� ����(*������ ��� Check_Key_File �Լ� ����)
	static HRESULT	StaticInit(bool bIsServer);

	/*
	��� ǥ�ý� ������ �ݵ�� \\�� �ٿ����Ѵ� ex)C:\\OpenSSL\\Key.pem
	CertFile�� Keyfile�� �ߺ��� �� �ִ�.
	*/
	static HRESULT	Confugure_Context(const char* CertFile, const char* KeyFile);
	~SSL_Utill();
	static void	ReportError(const char* szErrorMSG);
	static void	CleanUp();
	static SSL_CTX* Get_CTX(){
		return Instance()-> mCtx;
	}

	static TLS_SocketPtr	CreateTLS_Socket(SocketAddressFamily inFamily);
//	static TCPSocketPtr	CreateTCPSocket(SocketAddressFamily inFamily);

private:
	SSL_Utill();
	//�̱��� ����
	static shared_ptr<SSL_Utill>	Instance();
private:
	SSL_CTX*	mCtx;
	static  shared_ptr<SSL_Utill>	 m_pInstance;
	friend class TCPSocket;
};

/*---------------------------------------------------------------------------------------------/

*openssl ����ü ���� ����

SSL_CTX: ���� ����ü
	1. �������α׷��� 1��
	2. SSL ����ü���� �⺻���� �����ϸ�, ������ ���� �������.
	3. ������ ������ ���ǿ� ���� ��Ȳ ������ ����������
	�����̰ǵ� fd_list ó�� ���� ��ũ��Ʈ�� �������� ������ �ִٰ� select�� WSASelect ���� �Լ��� �̿��Ͽ� ��Ƽ �÷����� �Ұ� ����

SSL: SSL_CTX�� ��� �Ͽ� �������
	1.���ο� SSL ���Ḷ�� ����(SOCKET ���� �����ε�)
	2.	SSL_CTX, 
		SSL_METHOD. 
		SSL_SESSION(���� ����): ���� ��ȣȭ�� �ʿ��� ����
		SSL_CIPHER(��ȣȭ ����) :�˰��� ����
		CERT(������ ����) 
		BIO(����� ��Ʈ��):4������ TCP ������ �ٷ�

-----------------------------------------------------------------------------------------------*/
#endif