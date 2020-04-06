#pragma once
#ifdef PK_SOCK_EXPORTS
#define SOCK_DLL _declspec(dllexport)
#else
#define SOCK_DLL _declspec(dllimport)
#endif 

#ifdef _USE_SSL
#ifdef _DEBUG
#pragma comment(lib,"libssl64MTd.lib")
#pragma comment(lib,"libssl64MDd.lib")
#pragma comment(lib,"libcrypto64MTd.lib")
#pragma comment(lib,"libcrypto64MDd.lib")
#else
#pragma comment(lib,"libssl64MT.lib")
#pragma comment(lib,"libssl64MD.lib")
#pragma comment(lib,"libcrypto64MT.lib")
#pragma comment(lib,"libcrypto64MD.lib")
#endif
#endif
#include "PK_Shared.h"
