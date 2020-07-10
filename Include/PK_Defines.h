#pragma once
#define STREAM_ENDIANNESS 0
#define PLATFORM_ENDIANNESS 0

#ifndef PURE
#define PURE =0
#endif

enum SocketAddressFamily
{
	INET = AF_INET,
	INET6 = AF_INET6
};
