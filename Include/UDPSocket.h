#pragma once
class SOCK_DLL UDPSocket
	:public BaseSocket
{
public:
	int Bind(const SocketAddress& inToAddress);
	int SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress);
	int ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress);
	int SetNonBlockingMode(bool inShouldBeNonBlocking);


private:
	friend class SocketUtil;
	UDPSocket(SOCKET inSocket) : BaseSocket(inSocket) {}

};