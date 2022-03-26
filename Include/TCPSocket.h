#pragma once
class SOCK_DLL TCPSocket
	:public BaseSocket
{
public:
	enum ITER
	{
		READ_BLOCK,
		READ_ABLE,
		WRITE_BLOCK,
		WRITE_ABLE,
		NORMAL_BLOCK,
		ERR_ABLE,
		ITER_END
	};
	~TCPSocket()
	{
	}
public:
	int								Connect(const SocketAddress& inAddress);
	int								Bind(const SocketAddress& inToAddress);
	int								Listen(int inBackLog = 32);
	shared_ptr< TCPSocket >			Accept(SocketAddress& inFromAddress);
	int32_t							Send(const void* inData, size_t inLen);
	int32_t							Receive(void* inBuffer, size_t inLen);


	int								Disconnect();
	//SSL�� ������ ��� �ѹ� none_block���� ����ϸ� block���� ��ȯ�� �ȵ�
	int SetNonBlockingMode(bool inShouldBeNonBlocking);


private:
	friend class SocketUtil;
	TCPSocket(SOCKET inSocket) : BaseSocket(inSocket){}
};
