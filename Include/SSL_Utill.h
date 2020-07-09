#pragma once
#ifdef _USE_SSL
using namespace std;
class SOCK_DLL SSL_Utill
{

public:
	
	//반드시 소켓 초기화후 실행(*서버일 경우 Check_Key_File 함수 실행)
	static HRESULT	StaticInit(bool bIsServer);

	/*
	경로 표시시 폴더는 반드시 \\를 붙여야한다 ex)C:\\OpenSSL\\Key.pem
	CertFile과 Keyfile은 중복될 수 있다.
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
	//싱글톤 패턴
	static shared_ptr<SSL_Utill>	Instance();
private:
	SSL_CTX*	mCtx;
	static  shared_ptr<SSL_Utill>	 m_pInstance;
	friend class TCPSocket;
};

/*---------------------------------------------------------------------------------------------/

*openssl 구조체 대한 설명

SSL_CTX: 전역 구조체
	1. 응용프로그램당 1개
	2. SSL 구조체들의 기본값을 저장하며, 연결을 위해 만들어짐.
	3. 연결의 갯수와 세션에 대한 상황 정보를 가지고있음
	추측이건데 fd_list 처럼 파일 디스크립트의 정보들을 가지고 있다가 select나 WSASelect 같은 함수를 이용하여 멀티 플렉싱을 할것 같음

SSL: SSL_CTX를 상속 하여 만들어짐
	1.새로운 SSL 연결마다 생성(SOCKET 같은 역할인듯)
	2.	SSL_CTX, 
		SSL_METHOD. 
		SSL_SESSION(세션 정보): 실제 암호화에 필요한 정보
		SSL_CIPHER(암호화 정보) :알고리즘 정보
		CERT(인증서 정보) 
		BIO(입출력 스트림):4계층의 TCP 연결을 다룸

-----------------------------------------------------------------------------------------------*/
#endif