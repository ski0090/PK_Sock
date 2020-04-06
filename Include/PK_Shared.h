#pragma once
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <thread>

#pragma comment(lib,"ws2_32.lib")
typedef int socklen_t;
//typedef char* receiveBufer_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
//typedef void* receiveBufer_t;
typedef int SOCKET;
const int NO_ERROR = 0;
const int INVALID_SOCKET = -1;
const int WSAECONNRESET = ECONNRESET;
const int WSAEWOULDBLOCK = EAGAIN;
const int SOCKET_ERROR = -1;
#endif

#include <memory>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <windows.h>
#include <string>

#include <queue>
#include <deque>
#include <unordered_set>
#include <cassert>
#include <tchar.h>
#ifdef _USE_SSL
#include <openssl\ossl_typ.h>
#include <openssl\ssl.h>
#include <openssl\err.h>
#endif
//Headers
#include "PK_Usings.h"
#include "PK_defines.h"
#include "PK_ByteSwap.h"
#include "PK_structs.h"

//Network
#include "SocketAddress.h"
#include "BaseSocket.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "LinkingContext.h"
#include "MemoryStream.h"
#include "InStream.h"
#include "OutStream.h"

#ifdef _USE_SSL
#include <openssl\ossl_typ.h>
#include <openssl\ssl.h>
#include "TLS_Socket.h"
#endif
#include "SSL_Utill.h"
//System
#include "CriticalSection.h"
#include "PK_typedef.h"
#include "PK_externs.h"
//Utilities
#include "SocketUtil.h"
#include "StringUtils.h"
#include "SocketAddressFactory.h"

