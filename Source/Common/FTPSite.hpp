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

	// Use passive or active mode
	int Initialise( const bool p_Passive = true );

	// Connect using a domain name or IP address
	int Connect( const char *p_pAddress );
	int Disconnect( );

	// SendCommand just sends commands.  No command validation is performed.
	int SendCommand( const char *p_pCommand );
	int ReceiveData( char *p_pData );

	// Begin a file request and return the IP address and port to use
	int StartDataTransfer( std::string *p_pIPAddr, std::string *p_pPort );
	int ListCurrentDirectory( std::string *p_pResults );

private:
	void *GetINetAddress( const struct sockaddr *p_pAddress );
	int ConnectTo( SOCKET &p_Socket,
		const std::string p_IPAddress, const std::string p_Port );

	WSADATA m_WSA;
	SOCKET	m_Socket;

	bool m_SocketsInitialised;

	char *m_pServerAddress;
	bool m_IPv6;
	bool m_Passive;

	Log m_Log;
};

#endif
