#ifndef __UPDATER_FTPSITE_HPP__
#define __UPDATER_FTPSITE_HPP__

#ifndef _WINSOCK2API_
#include <winsock2.h>
#endif

#include <Log.hpp>
#include <list>

class FTPSite
{
public:
	FTPSite( );
	~FTPSite( );

	// Use passive or active mode
	int Initialise( const bool p_ActiveMode = false );

	// Intended to be called once by the user
	int Connect( );
	int Disconnect( );

	// SendCommand just sends commands.  No command validation is performed.
	int SendCommand( const char *p_pCommand );
	// Puts what the server returns into a buffer of the size specified
	int ReceiveData( char *p_pData, const size_t p_BufferSize = 1024 );

	// Begin a file request and return the IP address and port to use
	int StartDataTransfer( std::string *p_pIPAddr, std::string *p_pPort );

	// Abstractions for FTP's messages
	int ListCurrentDirectory( std::list< std::string > &p_Results );

	// Modifiers
	void SetPort( const char *p_pPort );
	void SetAddress( const char *p_pAddress );

private:
	// Returns the IPv4 or IPv6 address
	void *GetINetAddress( const struct sockaddr *p_pAddress );

	// Used internally and for the initial connect to the server
	int ConnectTo( SOCKET &p_Socket,
		const std::string p_IPAddress, const std::string p_Port );

	WSADATA m_WSA;
	SOCKET	m_Socket;

	bool m_SocketsInitialised;

	char *m_pServerAddress;
	char *m_pPort;
	bool m_IPv6;
	bool m_ActiveMode;

	Log m_Log;
};

#endif
