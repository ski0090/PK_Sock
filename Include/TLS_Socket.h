#pragma once
/*
	openssl 라이브러리가 base이다.
	TCPsocket 클래스의 컴포넌트로 활용한다.
*/
#ifdef _USE_SSL

class SOCK_DLL TLS_Socket
	:BaseSocket
{
public:
	~TLS_Socket();
	int32_t			Connect(const SocketAddress& inToAddress);
	int32_t			Bind(const SocketAddress& inToAddress);
	int32_t			Listen(int inBackLog = 32);
	shared_ptr<TLS_Socket>	Accept(SocketAddress& inToAddress);
	int32_t			Send(const void* inData, size_t inLen);
	int32_t			Receive(void* inBuffer, size_t inLen);
	int32_t			Disconnect();

	int32_t		SetNonBlockingMode(bool inShouldBeNonBlocking);

private:
	SSL* mSSL = nullptr;
	friend class SSL_Utill;
	TLS_Socket(SOCKET _socket);
	TLS_Socket(SOCKET _socket, SSL* _ssl);

};
using TLS_SocketPtr = shared_ptr<TLS_Socket>;
#endif