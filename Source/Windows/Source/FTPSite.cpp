#undef UNICODE
#include <FTPSite.hpp>
#include <ws2tcpip.h>
#include <iostream>

FTPSite::FTPSite( )
{
	m_SocketsInitialised = false;
	m_IPv6 = false;
	m_Log.SetLogFile( "ftp_information.log" );
}

FTPSite::~FTPSite( )
{
	if( m_SocketsInitialised == true )
	{
		closesocket( m_Socket );
		WSACleanup( );
	}
}

// Return either an IPv4 or IPv6 address
void *FTPSite::GetINetAddress( const struct sockaddr *p_pAddress )
{
	if ( p_pAddress->sa_family == AF_INET )
	{
		m_IPv6 = false;
		return &( ( ( struct sockaddr_in* )p_pAddress )->sin_addr );
	}
	m_IPv6 = true;
	return &( ( ( struct sockaddr_in6* )p_pAddress )->sin6_addr );
}

int FTPSite::Initialise( )
{
	WORD Version;
	Version = MAKEWORD( 2, 2 );

	int Error = WSAStartup( Version, &m_WSA );

	if( Error != 0 )
	{
		m_Log.Print( "[ERROR] Failed to initialise Windows Sockets API\n" );
		return 1;
	}

	if( LOBYTE( m_WSA.wVersion ) != 2 || HIBYTE( m_WSA.wVersion ) != 2 )
	{
		m_Log.Print( "[ERROR] Windows Sockets version does not match what was"
			" expected\n" );
		WSACleanup( );
		return 1;
	}

	m_SocketsInitialised = true;
	m_Log.Print( "[INFO] Initialised Windows Sockets\n" );
}

int FTPSite::Connect( const char *p_pAddress )
{
	if( p_pAddress == NULL || strlen( p_pAddress ) == 0 )
	{
		m_Log.Print( "[ERROR] Invalid address passed to Connect function\n" );
		return 1;
	}

	struct addrinfo Hints;
	memset( &Hints, 0, sizeof( Hints ) );
	Hints.ai_family = AF_UNSPEC;
	Hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *pServerInfo = NULL, *pBest = NULL;
	int Error = 0;

	if( Error = getaddrinfo( p_pAddress, "21", &Hints, &pServerInfo ) != 0 )
	{
		m_Log.Print( "[ERROR] Failed to get address information for %s\n",
			p_pAddress );
		return 1;
	}

	// Connect to the first result
	for( pBest = pServerInfo; pBest != NULL; pBest = pBest->ai_next )
	{
		if( ( m_Socket = socket( pBest->ai_family, pBest->ai_socktype,
			pBest->ai_protocol ) ) == -1 )
		{
			continue;
		}
		if( connect( m_Socket, pBest->ai_addr, pBest->ai_addrlen ) == -1 )
		{
			closesocket( m_Socket );
			continue;
		}
		break;
	}

	if( pBest == NULL )
	{
		m_Log.Print( "[ERROR] Failed to connect to server %s\n", p_pAddress );
		return 1;
	}

	char ServerStr[ INET6_ADDRSTRLEN ];

	inet_ntop( pBest->ai_family,
		this->GetINetAddress( ( struct sockaddr * )pBest->ai_addr ),
		ServerStr, sizeof( ServerStr ) );

	m_Log.Print( "[INFO] Connecting to %s:21 [%s]\n", ServerStr,
		m_IPv6 ? "IPv6" : "IPv4" );

	freeaddrinfo( pServerInfo );

	return 0;
}

int FTPSite::Disconnect( )
{
	m_Log.Print( "[INFO] Disconnecting\n" );
	closesocket( m_Socket );

	return 0;
}

int FTPSite::SendCommand( const char *p_pCommand )
{
	send( m_Socket, p_pCommand, strlen( p_pCommand ), 0 );
	m_Log.Print( "[INFO] Sending %s to server\n", p_pCommand );

	return 0;
}

int FTPSite::ReceiveData( )
{
	char Recv[ 1024 ] = { '\0' };
	memset( Recv, '\0', 1024 );
	recv( m_Socket, Recv, 1024, 0 );

	m_Log.Print( "[INFO] <Message from server>:\n%s\n", Recv );

	return 0;
}