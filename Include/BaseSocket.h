#pragma once
class BaseSocket
{
public:
	//논블로킹 I/O 모드 설정
	int SetNonBlockingMode(bool inShouldBeNonBlocking);

protected:
	friend class SocketUtil;

	BaseSocket(SOCKET inSocket) : mSocket(inSocket) {}
	~BaseSocket();
	SOCKET mSocket;
};

