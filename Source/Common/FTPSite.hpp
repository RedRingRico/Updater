#ifndef __UPDATER_FTPSITE_HPP__
#define __UPDATER_FTPSITE_HPP__

#ifndef _WINSOCK2API_
#include <winsock2.h>
#endif

class FTPSite
{
public:
	FTPSite( );
	~FTPSite( );

	int Initialise( );

	int Connect( );
	int Disconnect( );

	int SendMessage( );
	int ReceiveData( );

private:
	WSADATA m_WSA;

	SOCKET	m_Socket;

	bool m_SocketsInitialised;
};

#endif
