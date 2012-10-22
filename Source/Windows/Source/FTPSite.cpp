#undef UNICODE
#include <FTPSite.hpp>
#include <ws2tcpip.h>
#include <Utility.hpp>

FTPSite::FTPSite( )
{
	m_ActiveMode = false;
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

int FTPSite::ConnectTo( SOCKET &p_Socket, const std::string p_IPAddr,
	const std::string p_Port )
{
	if( p_IPAddr.empty( ) || p_Port.empty( ) )
	{
		m_Log.Print( "[ERROR] Server address or port was not provided\n" );
		return 1;
	}

	struct addrinfo *pServer = NULL, *pBest = NULL, Hints;

	memset( &Hints, 0, sizeof( Hints ) );

	Hints.ai_family = AF_UNSPEC;
	Hints.ai_socktype = SOCK_STREAM;

	if( getaddrinfo( p_IPAddr.c_str( ), p_Port.c_str( ),
		&Hints, &pServer ) != 0 )
	{
		m_Log.Print( "[ERROR] Failed to get address information for %s:%s\n",
			p_IPAddr.c_str( ), p_Port.c_str( ) );
		return 1;
	}

	for( pBest = pServer; pBest != NULL; pBest = pBest->ai_next )
	{
		if( ( p_Socket = socket( pBest->ai_family, pBest->ai_socktype,
			pBest->ai_protocol ) ) == -1 )
		{
			continue;
		}
		if( connect( p_Socket, pBest->ai_addr, pBest->ai_addrlen ) == -1 )
		{
			closesocket( p_Socket );
			continue;
		}
		break;
	}

	if( pBest == NULL )
	{
		m_Log.Print( "[ERROR] Failed to connect to %s:%s\n", p_IPAddr.c_str( ),
			p_Port.c_str( ) );
		
		freeaddrinfo( pServer );

		return 1;
	}

	char ServerStr[ INET6_ADDRSTRLEN ];

	inet_ntop( pBest->ai_family,
		this->GetINetAddress( ( struct sockaddr * )pBest->ai_addr ),
		ServerStr, sizeof( ServerStr ) );

	m_Log.Print( "[INFO] Connected to %s [%s]\n", ServerStr, 
		m_IPv6 ? "IPv6" : "IPv4" );

	freeaddrinfo( pServer );

	return 0;
}

int FTPSite::Initialise( const bool p_ActiveMode )
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

	m_ActiveMode = p_ActiveMode;

	return 0;
}

int FTPSite::Connect( const char *p_pAddress )
{
	if( p_pAddress == NULL || strlen( p_pAddress ) == 0 )
	{
		m_Log.Print( "[ERROR] Invalid address passed to Connect function\n" );
		return 1;
	}

	if( this->ConnectTo( m_Socket, p_pAddress, "21" ) != 0 )
	{
		m_Log.Print( "[ERROR] Could not create socket\n" );
		return 1;
	}

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
	m_Log.Print( "[INFO] Sending %s to server\n", p_pCommand );
	std::string Command( p_pCommand );
	Command.append( "\r\n" );
	send( m_Socket, Command.c_str( ), strlen( p_pCommand ), 0 );

	return 0;
}

int FTPSite::ReceiveData( char *p_pData )
{
	char Recv[ 1024 ] = { '\0' };
	memset( Recv, '\0', 1024 );
	recv( m_Socket, Recv, 1024, 0 );

	if( p_pData != NULL )
	{
		strncpy( p_pData, Recv, 1024 );
	}

	m_Log.Print( "[INFO] <Message from server>:\n%s\n", Recv );

	return 0;
}

int FTPSite::StartDataTransfer( std::string *p_pIPAddr, std::string *p_pPort )
{
	if( m_ActiveMode )
	{
	}
	else
	{
		this->SendCommand( "PASV\r\n" );
		char RawBuffer[ 1024 ] = { '\0' };
		this->ReceiveData( RawBuffer );

		std::string Buffer( RawBuffer );
		size_t StartBuff = 0, EndBuff = 0;

		StartBuff = Buffer.find_last_of( '(' );
		EndBuff = Buffer.find_last_of( ')' );

		Buffer = Buffer.substr( StartBuff+1, EndBuff-StartBuff-1 );

		// Extract the IP address
		std::string IPAddr;
		size_t CurPos = 0, PrevPos = 0;
		
		for( size_t i = 0; i < 4; ++i )
		{
			CurPos = Buffer.find( ',', PrevPos );
			IPAddr.append( Buffer.substr( PrevPos, CurPos-PrevPos ) );
			PrevPos = CurPos+1;
			if( i != 3 )
			{
				IPAddr.append( "." );
			}
		}

		// Extract the port
		std::string PortStrA, PortStrB;

		CurPos = Buffer.find( ',', PrevPos );
		PortStrA.append( Buffer.substr( PrevPos, CurPos-PrevPos ) );
		PrevPos = CurPos+1;

		CurPos = Buffer.find( ',', PrevPos );
		PortStrB.append( Buffer.substr( PrevPos, CurPos-PrevPos ) );

		int PortA = atoi( PortStrA.c_str( ) );
		int PortB = atoi( PortStrB.c_str( ) );

		int Port = ( PortA*256 ) + PortB;

		char PortStr[ 6 ];
		itoa( Port, PortStr, 10 );

		p_pIPAddr->clear( );
		p_pPort->clear( );
		p_pIPAddr->append( IPAddr );
		p_pPort->append( PortStr );
	}
	
	return 0;
}

int FTPSite::ListCurrentDirectory( std::list< std::string > &p_Results )
{
	std::string IPAddr, Port;

	this->StartDataTransfer( &IPAddr, &Port );

	m_Log.Print( "[INFO] Connecting to %s:%s\n",
		IPAddr.c_str( ), Port.c_str( ) );
	
	SOCKET Site;

	if( this->ConnectTo( Site, IPAddr, Port ) != 0 )
	{
		m_Log.Print( "[ERROR] Could not list directory\n" );
		return 1;
	}

	// Get the list
	this->SendCommand( "NLST\r\n" );

	char Recv[ 1024 ] = { '\0' };
	memset( Recv, '\0', 1024 );
	recv( Site, Recv, 1024, 0 );

	// Take the directories, ignore the current and parent directories
	size_t Items = 0;
	size_t CharStart = 0, CharEnd = 0;
	for( size_t i = 0; i < sizeof( Recv ); ++i )
	{
		if( Recv[ i ] == '\n' )
		{
			char *pTmpBuff = new char[ ( CharEnd-CharStart )+1 ];
			strncpy( pTmpBuff, Recv+CharStart, ( CharEnd-CharStart ) );
			pTmpBuff[ ( CharEnd-CharStart ) ] = '\0';

			// Remove any CRs
			for( size_t j = 0; j < strlen( pTmpBuff ); ++j )
			{
				if( pTmpBuff[ j ] == 0xD )
				{
					pTmpBuff[ j ] = '\0';
				}
			}

			if( ( strcmp( pTmpBuff, "." ) == 0 ) ||
				( strcmp( pTmpBuff, ".." ) == 0 ) )
			{

				SAFE_DELETE_ARRAY( pTmpBuff );
				CharStart = ++CharEnd;
				continue;
			}
			p_Results.push_back( pTmpBuff );
			CharStart = ++CharEnd;
			++Items;
			SAFE_DELETE_ARRAY( pTmpBuff );
			continue;
		}
		if( Recv[ i ] == '\0' )
		{
			break;
		}
		++CharEnd;
	}

	m_Log.Print( "[INFO] <Message from server>:\n%s\n", Recv );

	this->ReceiveData( NULL );

	closesocket( Site );

	return 0;
}
