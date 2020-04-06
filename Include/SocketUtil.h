#pragma once

class SOCK_DLL SocketUtil
{
public:

	static bool			StaticInit();
	static void			CleanUp();

	static void			ReportError(const char* inOperationDesc);
	static int			GetLastError();

	static int			Select(const list< TCPSocketPtr >* inReadSet,
		list< TCPSocketPtr >* outReadSet,
		const list< TCPSocketPtr >* inWriteSet,
		list< TCPSocketPtr >* outWriteSet,
		const list< TCPSocketPtr >* inExceptSet,
		list< TCPSocketPtr >* outExceptSet);

	static int			Select(const vector< TCPSocketPtr >* inReadSet,
		vector< TCPSocketPtr >* outReadSet,
		const vector< TCPSocketPtr >* inWriteSet,
		vector< TCPSocketPtr >* outWriteSet,
		const vector< TCPSocketPtr >* inExceptSet,
		vector< TCPSocketPtr >* outExceptSet);

	static UDPSocketPtr	CreateUDPSocket(SocketAddressFamily inFamily);
	static TCPSocketPtr	CreateTCPSocket(SocketAddressFamily inFamily);

private:

	inline static fd_set* FillSetFrom_STL(fd_set& outSet, const list< TCPSocketPtr >* inSockets, int& ioNaxNfds);
	inline static void Fill_STLFromSet(TCPSocket::ITER eIter, list< TCPSocketPtr >* outSockets, const list< TCPSocketPtr >* inSockets, const fd_set& inSet);
	inline static fd_set* FillSetFrom_STL(fd_set& outSet, const vector< TCPSocketPtr >* inSockets, int& ioNaxNfds);
	inline static void Fill_STLFromSet(vector< TCPSocketPtr >* outSockets, const vector< TCPSocketPtr >* inSockets, const fd_set& inSet);
};