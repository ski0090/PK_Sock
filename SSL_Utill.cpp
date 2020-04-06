//Secret
#ifdef _USE_SSL

#include "PK_Sock.h"
#include <openssl\ossl_typ.h>
#include <openssl\bio.h>
#include <openssl\ssl.h>
#include <openssl\err.h>
#include <openssl\engine.h>
#include <openssl\conf.h>

shared_ptr<SSL_Utill> SSL_Utill::m_pInstance;
SSL_Utill::SSL_Utill() :mCtx(nullptr)
{
}


shared_ptr<SSL_Utill> SSL_Utill::Instance()
{
	if (m_pInstance.get() == nullptr)
	{
		m_pInstance.reset(new SSL_Utill);
	}
	return m_pInstance;
}

HRESULT SSL_Utill::StaticInit(bool bIsServer)
{
	if (SSL_Utill::Instance()->mCtx)
	{
		ReportError("Open SSL Release Is Already");
		return FALSE;
	}
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_Utill::Instance()->mCtx
		= SSL_CTX_new((bIsServer ? TLS_server_method() : TLS_client_method()));
	if (SSL_Utill::Instance()->mCtx == nullptr)
	{
		ERR_print_errors_fp(stderr);
		StringUtils::Log("오픈 SSL 초기화 실패!");
		return FALSE;
	}

	return TRUE;
}

HRESULT SSL_Utill::Confugure_Context(const char* CertFile, const char* KeyFile)
{
	SSL_CTX_set_ecdh_auto(mCtx, 1);
	//인증서 파일의 정보를 ctx로 가지고 옴
	if (SSL_CTX_use_certificate_file(SSL_Utill::Instance()->mCtx, CertFile, SSL_FILETYPE_PEM) <= 0)
	{
		ReportError("Cert File Error!");
		return FALSE;
	}
	/* 개인키를 키파일에서 추출하여 ctx에 추가함*/
	if (SSL_CTX_use_PrivateKey_file(SSL_Utill::Instance()->mCtx, KeyFile, SSL_FILETYPE_PEM) <= 0)
	{
		ReportError("Provate Key File Error!");
		return FALSE;
	}
	/* 인증서 파일의 정보와 개인키의 정보의 일관성을 검사 */
	if (!SSL_CTX_check_private_key(SSL_Utill::Instance()->mCtx))
	{
		ReportError("Private key does not match the public certificate\n");
		return FALSE;
	}
	return TRUE;
}

void SSL_Utill::ReportError(const char* szErrorMSG)
{
	char lpMsgBuf[256] = {0,};
	DWORD errorNum = ERR_peek_last_error();
	ERR_error_string(errorNum,lpMsgBuf);
	StringUtils::Log("Error %s: %d- %s", szErrorMSG, errorNum, lpMsgBuf);
}

void SSL_Utill::CleanUp()
{
	m_pInstance.reset();
}

TLS_SocketPtr SSL_Utill::CreateTLS_Socket(SocketAddressFamily inFamily)
{
	SOCKET s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);
	if (s != INVALID_SOCKET)
	{
		return TLS_SocketPtr(new TLS_Socket(s));
	}
	else
	{
		ReportError("SocketUtil::CreateTCPSocket");
		return nullptr;
	}
}

SSL_Utill::~SSL_Utill()
{
	SSL_CTX_free(mCtx);
	ERR_free_strings();
	EVP_cleanup();
}

#endif