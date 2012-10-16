#undef UNICODE
#include <FTPSite.hpp>
#include <ws2tcpip.h>
#include <iostream>

FTPSite::FTPSite( )
{
	m_SocketsInitialised = false;
}

FTPSite::~FTPSite( )
{
	if( m_SocketsInitialised == true )
	{
		closesocket( m_Socket );
		WSACleanup( );
	}
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int FTPSite::Initialise( )
{
	WORD Version;
	Version = MAKEWORD( 2, 2 );

	int Error = WSAStartup( Version, &m_WSA );

	if( Error != 0 )
	{
		return 1;
	}

	if( LOBYTE( m_WSA.wVersion ) != 2 || HIBYTE( m_WSA.wVersion ) != 2 )
	{
		WSACleanup( );
		return 1;
	}

	m_SocketsInitialised = true;
	
}

int FTPSite::Connect( )
{
	// Connect to hard-coded server
	struct addrinfo Hints;
	memset( &Hints, 0, sizeof( Hints ) );
	Hints.ai_family = AF_UNSPEC;
	Hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *pServerInfo = NULL, *pBest = NULL;
	int Error = 0;

	if( Error = getaddrinfo( "95.154.194.27", "21", &Hints, &pServerInfo ) != 0 )
	{
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
		return 1;
	}

	char s[ INET6_ADDRSTRLEN ];

	inet_ntop( pBest->ai_family, get_in_addr( ( struct sockaddr * )pBest->ai_addr ),
		s, sizeof( s ) );

	std::cout << "Connecting to " << s << std::endl;
	freeaddrinfo( pServerInfo );
}

int FTPSite::Disconnect( )
{
	std::cout << "Disconnecting" << std::endl;
	closesocket( m_Socket );
	return 0;
}
