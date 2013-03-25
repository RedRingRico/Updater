#ifdef PLATFORM_WINDOWS
#undef UNICODE
#define WINVER 0x0600
#define _WIN32_WINNT 0x0600
#endif

#ifdef PLATFORM_LINUX
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#endif

#include <FTPSite.hpp>

#ifdef PLATFORM_WINDOWS
#include <ws2tcpip.h>
#endif

#include <Utility.hpp>

#ifdef PLATFORM_WINDOWS
#ifdef __GNUC__
// Credit for inet_ntop2
// http://memset.wordpress.com/2010/10/09/inet_ntop-for-win32
const char* inet_ntop2( int af, const void* src, char* dst, int cnt )
{
    struct sockaddr_in srcaddr;
 
    memset( &srcaddr, 0, sizeof( struct sockaddr_in ) );
    memcpy( &( srcaddr.sin_addr ), src, sizeof( srcaddr.sin_addr ) );
 
    srcaddr.sin_family = af;
    if( WSAAddressToString( (struct sockaddr* ) &srcaddr,
	 sizeof(struct sockaddr_in), 0, dst, ( LPDWORD )&cnt ) != 0 )
	{
        DWORD rv = WSAGetLastError( );
        printf( "WSAAddressToString( ) : %d\n", rv );
        return NULL;
    }
    return dst;
}
#endif
#endif

FTPSite::FTPSite( )
{
	m_ActiveMode = false;
	m_SocketsInitialised = false;
	m_IPv6 = false;
	m_pServerAddress = NULL;
	m_pPort = NULL;
	m_Log.SetLogFile( "ftp_information.log" );
}

FTPSite::~FTPSite( )
{
	if( m_pServerAddress )
	{
		SAFE_DELETE_ARRAY( m_pPort );
	}
	if( m_pPort )
	{
		SAFE_DELETE_ARRAY( m_pPort );
	}
	if( m_SocketsInitialised == true )
	{
#ifdef PLATFORM_WINDOWS
		closesocket( m_Socket );
		WSACleanup( );
#else
		close( m_Socket );
#endif
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
#ifdef PLATFORM_WINDOWS
			closesocket( p_Socket );
#else
			close( p_Socket );
#endif
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

#ifdef PLATFORM_WINDOWS
#ifdef __GNUC__
	inet_ntop2( pBest->ai_family,
		this->GetINetAddress( ( struct sockaddr * )pBest->ai_addr ),
		ServerStr, sizeof( ServerStr ) );
#else
	inet_ntop( pBest->ai_family,
		this->GetINetAddress( ( struct sockaddr * )pBest->ai_addr ),
		ServerStr, sizeof( ServerStr ) );
#endif
#else
	inet_ntop( pBest->ai_family,
		this->GetINetAddress( ( struct sockaddr * )pBest->ai_addr ),
		ServerStr, sizeof( ServerStr ) );
#endif

	m_Log.Print( "[INFO] Connected to %s [%s]\n", ServerStr, 
		m_IPv6 ? "IPv6" : "IPv4" );

	// Send the commands to begin interacting with the FTP site

	freeaddrinfo( pServer );

	return 0;
}

int FTPSite::Initialise( const bool p_ActiveMode )
{
#ifdef PLATFORM_WINDOWS
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
#endif
	m_SocketsInitialised = true;
	m_Log.Print( "[INFO] Initialised Sockets\n" );

	m_ActiveMode = p_ActiveMode;

	return 0;
}

int FTPSite::Connect( )
{
	if( m_pServerAddress == NULL || strlen( m_pServerAddress ) == 0 )
	{
		m_Log.Print( "[ERROR] Invalid address passed to Connect function\n" );
		return 1;
	}

	if( m_pPort == NULL || strlen( m_pPort ) == 0 )
	{
		m_pPort = new char[ 3 ];
		strncpy( m_pPort, "21\0", 3 );
	}

	if( this->ConnectTo( m_Socket, m_pServerAddress, m_pPort ) != 0 )
	{
		m_Log.Print( "[ERROR] Could not create socket\n" );
		return 1;
	}

	// Send the login details for the main client part of the connection,
	// get some details about the server, and set the transmission type to
	// 8-bit binary
	this->ReceiveData( NULL );
	this->SendCommand( "USER anonymous" );
	this->ReceiveData( NULL );
	this->SendCommand( "PASS anonymous@anyone.com" );
	this->ReceiveData( NULL );
	this->SendCommand( "SYST" );
	this->ReceiveData( NULL );
	this->SendCommand( "FEAT" );
	this->ReceiveData( NULL );
	this->SendCommand( "TYPE I" );
	this->ReceiveData( NULL );

	return 0;
}

int FTPSite::Disconnect( )
{
	m_Log.Print( "[INFO] Disconnecting\n" );
	this->SendCommand( "QUIT" );
	this->ReceiveData( NULL );

#ifdef PLATFORM_WINDOWS
	closesocket( m_Socket );
#else
	close( m_Socket );
#endif

	return 0;
}

int FTPSite::SendCommand( const char *p_pCommand )
{
	m_Log.Print( "[INFO] Sending %s to server\n", p_pCommand );
	std::string Command( p_pCommand );
	Command.append( "\r\n" );
	send( m_Socket, Command.c_str( ), Command.size( ), 0 );

	return 0;
}

int FTPSite::ReceiveData( char *p_pData, const size_t p_BufferSize )
{
	char *pRecv = new char [ p_BufferSize ];
	memset( pRecv, '\0', p_BufferSize );

	recv( m_Socket, pRecv, p_BufferSize, 0 );

	if( p_pData != NULL )
	{
		strncpy( p_pData, pRecv, strlen( pRecv ) );
		p_pData[ strlen( pRecv ) ] = '\0';
	}
	m_Log.Print( "[INFO] <Message from server>:\n%s\n", pRecv );

	SAFE_DELETE_ARRAY( pRecv );

	return 0;
}

int FTPSite::StartDataTransfer( std::string *p_pIPAddr, std::string *p_pPort )
{
	if( m_ActiveMode )
	{
	}
	else
	{
		// The IP Address and Port are in the format:
		// a,a,a,a,pa,pb
		// Where a is a dotted-decimal version of the IP address and pa and pb
		// are combined to create the port - ( pa*256 ) + pb
		this->SendCommand( "PASV\r\n" );
		char RawBuffer[ 1024 ] = { '\0' };
		memset( RawBuffer, '\0', 1024 );
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

#ifdef CPP11
		std::string PortStr;
		PortStr = std::to_string( Port );
#else
		char PortStr[ 6 ];
		itoa( Port, PortStr, 10 );
#endif

		p_pIPAddr->clear( );
		p_pPort->clear( );
		p_pIPAddr->append( IPAddr );
		p_pPort->append( PortStr );
	}
	
	return 0;
}

int FTPSite::ListCurrentDirectory( std::list< std::string > &p_Results )
{
	if( m_ActiveMode )
	{
	}
	else
	{
		std::string IPAddr, Port;

		this->StartDataTransfer( &IPAddr, &Port );

		m_Log.Print( "[INFO] Connecting to %s:%s\n", IPAddr.c_str( ),
			Port.c_str( ) );
	
		SOCKET Site;

		if( this->ConnectTo( Site, IPAddr, Port ) != 0 )
		{
			m_Log.Print( "[ERROR] Could not list directory\n" );
			return 1;
		}

		// Get the list
		this->SendCommand( "NLST" );

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

#ifdef PLATFORM_WINDOWS
		closesocket( Site );
#else
		close( Site );
#endif
	}

	return 0;
}

void FTPSite::SetPort( const char *p_pPort )
{
	if( p_pPort == NULL || strlen( p_pPort ) == 0 )
	{
		return;
	}

	m_pPort = new char[ strlen( p_pPort ) + 1 ];
	strncpy( m_pPort, p_pPort, strlen( p_pPort ) );
	m_pPort[ strlen( p_pPort ) ] = '\0';
}

void FTPSite::SetAddress( const char *p_pAddress )
{
	if( p_pAddress == NULL || strlen( p_pAddress ) == 0 )
	{
		return;
	}

	m_pServerAddress = new char[ strlen( p_pAddress ) + 1 ];
	strncpy( m_pServerAddress, p_pAddress, strlen( p_pAddress ) );
	m_pServerAddress[ strlen( p_pAddress ) ] = '\0';
}

