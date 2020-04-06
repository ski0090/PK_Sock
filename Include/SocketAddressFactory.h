#pragma once
class SOCK_DLL SocketAddressFactory
{
public:
	static SocketAddressPtr CreateIPv4FromString(const string& inString);
};

