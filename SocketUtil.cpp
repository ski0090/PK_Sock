#include "PK_Sock.h"
bool SocketUtil::StaticInit()
{
#if _WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		ReportError("Starting Up");
		return false;
	}
#endif

	return true;
}

void SocketUtil::CleanUp()
{
#if _WIN32
	WSACleanup();
#endif
}


void SocketUtil::ReportError(const char* inOperationDesc)
{
#if _WIN32
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);


	LOG("Error %s: %d- %s", inOperationDesc, errorNum, lpMsgBuf);
#else
	LOG("Error: %hs", inOperationDesc);
#endif
}

int SocketUtil::GetLastError()
{
#if _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif

}

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily)
{
	SOCKET s = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);

	if (s != INVALID_SOCKET)
	{
		return UDPSocketPtr(new UDPSocket(s));
	}
	else
	{
		ReportError("SocketUtil::CreateUDPSocket");
		return nullptr;
	}
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily)
{
	SOCKET s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);

	if (s != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(s));
	}
	else
	{
		ReportError("SocketUtil::CreateTCPSocket");
		return nullptr;
	}
}

fd_set* SocketUtil::FillSetFrom_STL(fd_set& outSet, const list< TCPSocketPtr >* inSockets, int& ioNaxNfds)
{
	if (inSockets)
	{
		FD_ZERO(&outSet);//fd_set* �� ����Ű�� ������ ZeroMemory�Ѵ�.

		for (const TCPSocketPtr& socket : *inSockets)
		{
			FD_SET(socket->mSocket, &outSet);
			//FD_SET(����,fdset)fdset�� ����Ű�� ������ �������� �����Ѵ�. ��, ������ �����ϰڴٴ� ��
#if !_WIN32
			ioNaxNfds = std::max(ioNaxNfds, socket->mSocket);
#endif
		}
		return &outSet;
	}
	else
	{
		return nullptr;
	}
}

fd_set* SocketUtil::FillSetFrom_STL(fd_set& outSet, const vector< TCPSocketPtr >* inSockets, int& ioNaxNfds)
{
	if (inSockets)
	{
		FD_ZERO(&outSet);//fd_set* �� ����Ű�� ������ ZeroMemory�Ѵ�.

		for (const TCPSocketPtr& socket : *inSockets)
		{
			FD_SET(socket->mSocket, &outSet);
			//FD_SET(����,fdset)fdset�� ����Ű�� ������ �������� �����Ѵ�. ��, ������ �����ϰڴٴ� ��
#if !_WIN32
			ioNaxNfds = std::max(ioNaxNfds, socket->mSocket);
#endif
		}
		return &outSet;
	}
	else
	{
		return nullptr;
	}
}

void SocketUtil::Fill_STLFromSet(TCPSocket::ITER eIter,list< TCPSocketPtr >* outSockets, const list< TCPSocketPtr >* inSockets, const fd_set& inSet)
{
	if (inSockets && outSockets)
	{
		outSockets->clear();
		for (const TCPSocketPtr& socket : *inSockets)
		{
			//
			if (FD_ISSET(socket->mSocket, &inSet))
			{
				outSockets->push_back(socket);
			}
		}
	}
}

void SocketUtil::Fill_STLFromSet(vector< TCPSocketPtr >* outSockets, const vector< TCPSocketPtr >* inSockets, const fd_set& inSet)
{
	if (inSockets && outSockets)
	{
		outSockets->clear();
		for (const TCPSocketPtr& socket : *inSockets)
		{
			//
			if (FD_ISSET(socket->mSocket, &inSet))
			{
				outSockets->push_back(socket);
			}
		}
	}
}

int SocketUtil::Select(const list< TCPSocketPtr >* inReadSet,
	list< TCPSocketPtr >* outReadSet,
	const list< TCPSocketPtr >* inWriteSet,
	list< TCPSocketPtr >* outWriteSet,
	const list< TCPSocketPtr >* inExceptSet,
	list< TCPSocketPtr >* outExceptSet)
{
	//build up some sets from our vectors
	fd_set read, write, except;

	int nfds = 0;
	/*
		fd_set �� 0�� 1�� ��Ÿ���� ��Ʈ�� �迭�̶� �����ϸ� �ȴ�.
		0:���� ����� �ƴ�
		1:���� ���
	*/
	fd_set *readPtr = FillSetFrom_STL(read, inReadSet, nfds);
	fd_set *writePtr = FillSetFrom_STL(write, inWriteSet, nfds);
	fd_set *exceptPtr = FillSetFrom_STL(except, inExceptSet, nfds);
	timeval tVal{ 5, 0 };
	int toRet = select(nfds + 1, //�˻� ����� �Ǵ� ���� ��ũ������ ��
		readPtr,//������ �����͸� ���ϰ� �ִ� ������ �����ϴ°�? 
				//�Է� ���ۿ� �����Ͱ� ���� �ְų�,���� ��û�� �ִ� ����.
		writePtr,	//�����͸� ������ ��� ���ŷ ���� �ʴ� ������ �����ΰ�?
					//��� ���ۿ� ������ �ִ� ����
		exceptPtr,	//���ܻ�Ȳ�� �߻��� ������ �ִ°�?
					//Out-of-band Data�� ���� �� ����??
		nullptr);//�Լ� ȣ�� ��,���� ��� ���¿� ������ �ʰ�� �����ϴ� �Ű�����
				 //timeval(
					//tv_sec,	�� ����
					//tv_usec)	����ũ�� �� ���� 1000000 =1��

	if (toRet > 0)
	{
		Fill_STLFromSet(TCPSocket::READ_ABLE ,outReadSet, inReadSet, read);
		Fill_STLFromSet(TCPSocket::WRITE_ABLE, outWriteSet, inWriteSet, write);
		Fill_STLFromSet(TCPSocket::ERR_ABLE,outExceptSet, inExceptSet, except);
	}
	return toRet;
}

int SocketUtil::Select(const vector< TCPSocketPtr >* inReadSet, vector< TCPSocketPtr >* outReadSet, const vector< TCPSocketPtr >* inWriteSet, vector< TCPSocketPtr >* outWriteSet, const vector< TCPSocketPtr >* inExceptSet, vector< TCPSocketPtr >* outExceptSet)
{
	//build up some sets from our vectors
	fd_set read, write, except;

	int nfds = 0;
	/*
	fd_set �� 0�� 1�� ��Ÿ���� ��Ʈ�� �迭�̶� �����ϸ� �ȴ�.
	0:���� ����� �ƴ�
	1:���� ���
	*/
	fd_set *readPtr = FillSetFrom_STL(read, inReadSet, nfds);
	fd_set *writePtr = FillSetFrom_STL(write, inWriteSet, nfds);
	fd_set *exceptPtr = FillSetFrom_STL(except, inExceptSet, nfds);
	timeval tVal{ 5, 0 };
	int toRet = select(nfds + 1, //�˻� ����� �Ǵ� ���� ��ũ������ ��
		readPtr,//������ �����͸� ���ϰ� �ִ� ������ �����ϴ°�? 
		//�Է� ���ۿ� �����Ͱ� ���� �ְų�,���� ��û�� �ִ� ����.
		writePtr,	//�����͸� ������ ��� ���ŷ ���� �ʴ� ������ �����ΰ�?
		//��� ���ۿ� ������ �ִ� ����
		exceptPtr,	//���ܻ�Ȳ�� �߻��� ������ �ִ°�?
		//Out-of-band Data�� ���� �� ����??
		nullptr);//�Լ� ȣ�� ��,���� ��� ���¿� ������ �ʰ�� �����ϴ� �Ű�����
	//timeval(
	//tv_sec,	�� ����
	//tv_usec)	����ũ�� �� ���� 1000000 =1��

	if (toRet > 0)
	{
		Fill_STLFromSet(outReadSet, inReadSet, read);
		Fill_STLFromSet(outWriteSet, inWriteSet, write);
		Fill_STLFromSet(outExceptSet, inExceptSet, except);
	}
	return toRet;
}
