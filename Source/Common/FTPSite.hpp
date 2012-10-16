#ifndef __UPDATER_FTPSITE_HPP__
#define __UPDATER_FTPSITE_HPP__

#ifndef _WINSOCK2API_
#include <winsock2.h>
#endif

#include <Log.hpp>

class FTPSite
{
public:
	FTPSite( );
	~FTPSite( );

	int Initialise( );

	// Connect using a domain name or IP address
	int Connect( const char *p_pAddress );
	int Disconnect( );

	int SendCommand( const char *p_pCommand );
	int ReceiveData( );

private:
	void *GetINetAddress( const struct sockaddr *p_pAddress );


	WSADATA m_WSA;
	SOCKET	m_Socket;

	bool m_SocketsInitialised;

	char *m_pServerAddress;
	bool m_IPv6;

	Log m_Log;
};

#endif
