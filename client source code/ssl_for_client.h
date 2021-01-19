#ifndef _SSL_FOR_CLIENT_H_
#define _SSL_FOR_CLIENT_H_

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace CUSTOM
{
    SSL_CTX* InitClientCTX();
    SSL_CTX* InitServerCTX();
    void LoadCertificates(SSL_CTX* ctx); // , const char* CertFile, const char* KeyFile
    void ShowCerts(SSL *ssl);
}

#endif