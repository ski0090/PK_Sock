#pragma once
class BaseSocket
{
public:
	//����ŷ I/O ��� ����
	int SetNonBlockingMode(bool inShouldBeNonBlocking);

protected:
	friend class SocketUtil;

	BaseSocket(SOCKET inSocket) : mSocket(inSocket) {}
	~BaseSocket();
	SOCKET mSocket;
};

