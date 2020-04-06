#include "PK_Sock.h"
#ifdef _USE_SSL

#define  MATCHING_FAIL -1
TLS_Socket::~TLS_Socket()
{
	if (mSSL)
		SSL_free(mSSL);
}

int32_t TLS_Socket::Connect(const SocketAddress& inToAddress)
{
	int err = connect(mSocket, &inToAddress.mSockAddr, inToAddress.GetSize());
	if (SSL_connect(mSSL) == MATCHING_FAIL)
	{
		SSL_Utill::ReportError("Connect Error");
		return E_FAIL;
	}
	return S_OK;
}

int32_t TLS_Socket::Bind(const SocketAddress& inToAddress)
{
	int error = ::bind(mSocket, &inToAddress.mSockAddr, inToAddress.GetSize());
	if (error != 0)
	{
		SocketUtil::ReportError("TLS_Socket::Bind");
		return SocketUtil::GetLastError();
	}
}

int32_t TLS_Socket::Listen(int inBackLog /*= 32*/)
{
	int err = listen(mSocket, inBackLog);
	if (err < 0)
	{
		SocketUtil::ReportError("TLS_Socket::Listen");
		return -SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

TLS_SocketPtr TLS_Socket::Accept(SocketAddress& inToAddress)
{
	SSL_CTX* mainCTX = SSL_Utill::Get_CTX(); 
	if (mainCTX == nullptr)
		return nullptr;

	socklen_t length = inToAddress.GetSize();
	SOCKET newSocket = accept(mSocket, &inToAddress.mSockAddr, &length);

	TLS_Socket* pTls = nullptr;
	if (newSocket != INVALID_SOCKET)
	{
	
		SSL* pSSL = SSL_new(SSL_Utill::Get_CTX());
		pTls = new TLS_Socket(newSocket, pSSL);
		if (SSL_accept(pSSL) == MATCHING_FAIL)
		{
			SSL_Utill::ReportError("TLS_Socket::Accept Error");
			delete pTls;
			return nullptr;
		}
	}

	X509 *cert;
	char *line;
	cert = SSL_get_peer_certificate(mSSL); /* 서버에 대한 인증서 정보를 가져온다.*/
	if (cert != NULL)
	{
		LOG("Server certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		LOG("Subject: %s\n", line);
		free(line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		LOG("Issuer: %s\n", line);
		free(line);
		X509_free(cert);
	}
	else
		LOG("No certificates.\n");

	return TLS_SocketPtr(pTls);
}

int32_t TLS_Socket::Send(const void* inData, size_t inLen)
{
	int bytesSentCount = SSL_write(mSSL, inData, inLen);
	if (bytesSentCount < 0)
	{
		SocketUtil::ReportError("TLS_Socket::Send");
		return -SocketUtil::GetLastError();
	}
	return bytesSentCount;
}

int32_t TLS_Socket::Receive(void* inBuffer, size_t inLen)
{
	int bytesSentCount = SSL_read(mSSL, inBuffer, inLen);
	if (bytesSentCount < 0)
	{
		SocketUtil::ReportError("TLS_Socket::Receive");
		return -SocketUtil::GetLastError();
	}
	return bytesSentCount;

}

int32_t TLS_Socket::Disconnect()
{
	int bytesReceivedCount = closesocket(mSocket);
	if (mSSL)
		SSL_free(mSSL);

	if (bytesReceivedCount != INVALID_SOCKET)
	{
		SocketUtil::ReportError("TLS_Socket::Disconnect");
		return SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

int32_t TLS_Socket::SetNonBlockingMode(bool inShouldBeNonBlocking)
{
	return BaseSocket::SetNonBlockingMode(inShouldBeNonBlocking);
}

TLS_Socket::TLS_Socket(SOCKET _socket)
	: BaseSocket(_socket)
{
	mSSL = SSL_new(SSL_Utill::Get_CTX());
	SSL_set_fd(mSSL,mSocket);
}

TLS_Socket::TLS_Socket(SOCKET _socket, SSL* _ssl)
	:BaseSocket(_socket), mSSL(_ssl)
{
	SSL_set_fd(mSSL, mSocket);
}
#endif