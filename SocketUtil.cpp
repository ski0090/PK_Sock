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
		FD_ZERO(&outSet);//fd_set* 가 가르키는 변수를 ZeroMemory한다.

		for (const TCPSocketPtr& socket : *inSockets)
		{
			FD_SET(socket->mSocket, &outSet);
			//FD_SET(소켓,fdset)fdset이 가리키는 변수에 소켓으로 설정한다. 즉, 소켓을 관찰하겠다는 뜻
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
		FD_ZERO(&outSet);//fd_set* 가 가르키는 변수를 ZeroMemory한다.

		for (const TCPSocketPtr& socket : *inSockets)
		{
			FD_SET(socket->mSocket, &outSet);
			//FD_SET(소켓,fdset)fdset이 가리키는 변수에 소켓으로 설정한다. 즉, 소켓을 관찰하겠다는 뜻
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
		fd_set 은 0과 1을 나타내는 비트의 배열이라 생각하면 된다.
		0:관찰 대상이 아님
		1:관찰 대상
	*/
	fd_set *readPtr = FillSetFrom_STL(read, inReadSet, nfds);
	fd_set *writePtr = FillSetFrom_STL(write, inWriteSet, nfds);
	fd_set *exceptPtr = FillSetFrom_STL(except, inExceptSet, nfds);
	timeval tVal{ 5, 0 };
	int toRet = select(nfds + 1, //검사 대상이 되는 파일 디스크립터의 수
		readPtr,//수신할 데이터를 지니고 있는 소켓이 존재하는가? 
				//입력 버퍼에 데이터가 남아 있거나,연결 요청이 있는 소켓.
		writePtr,	//데이터를 전송할 경우 블로킹 되지 않는 소켓은 무엇인가?
					//출력 버퍼에 여유가 있는 소켓
		exceptPtr,	//예외상황이 발상한 소켓이 있는가?
					//Out-of-band Data가 수신 된 소켓??
		nullptr);//함수 호출 후,무한 대기 상태에 빠지지 않고록 설정하는 매개변수
				 //timeval(
					//tv_sec,	초 단위
					//tv_usec)	마이크로 초 단위 1000000 =1초

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
	fd_set 은 0과 1을 나타내는 비트의 배열이라 생각하면 된다.
	0:관찰 대상이 아님
	1:관찰 대상
	*/
	fd_set *readPtr = FillSetFrom_STL(read, inReadSet, nfds);
	fd_set *writePtr = FillSetFrom_STL(write, inWriteSet, nfds);
	fd_set *exceptPtr = FillSetFrom_STL(except, inExceptSet, nfds);
	timeval tVal{ 5, 0 };
	int toRet = select(nfds + 1, //검사 대상이 되는 파일 디스크립터의 수
		readPtr,//수신할 데이터를 지니고 있는 소켓이 존재하는가? 
		//입력 버퍼에 데이터가 남아 있거나,연결 요청이 있는 소켓.
		writePtr,	//데이터를 전송할 경우 블로킹 되지 않는 소켓은 무엇인가?
		//출력 버퍼에 여유가 있는 소켓
		exceptPtr,	//예외상황이 발상한 소켓이 있는가?
		//Out-of-band Data가 수신 된 소켓??
		nullptr);//함수 호출 후,무한 대기 상태에 빠지지 않고록 설정하는 매개변수
	//timeval(
	//tv_sec,	초 단위
	//tv_usec)	마이크로 초 단위 1000000 =1초

	if (toRet > 0)
	{
		Fill_STLFromSet(outReadSet, inReadSet, read);
		Fill_STLFromSet(outWriteSet, inWriteSet, write);
		Fill_STLFromSet(outExceptSet, inExceptSet, except);
	}
	return toRet;
}
