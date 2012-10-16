#include <WinSock2.h>
#include <iostream>
#include <Log.hpp>
#include <FTPSite.hpp>
#include <conio.h>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Updater Version 0.0.0.1 [Prototype]" << std::endl;           

	FTPSite Site;

	if( Site.Initialise( ) != 0 )
	{
		return 1;
	}
	if( Site.Connect( "95.154.194.27" ) != 0 )
	{
		return 1;
	}

	Site.ReceiveData( NULL );
	Site.SendCommand( "USER anonymous\r\n" );
	Site.ReceiveData( NULL );
	Site.SendCommand( "PASS anonymous@anyone.com\r\n" );
	Site.ReceiveData( NULL );
	Site.SendCommand( "SYST\r\n" );
	Site.ReceiveData( NULL );
	//Site.SendCommand( "AUTH TLS\r\n" );
	//Site.ReceiveData( );
	Site.SendCommand( "FEAT\r\n" );
	Site.ReceiveData( NULL );
	Site.SendCommand( "TYPE I\r\n" );
	Site.ReceiveData( NULL );
	//Site.SendCommand( "PORT 192,168,2,16,57,218\r\n" );
	//Site.ReceiveData( );
	/*Site.SendCommand( "PASV\r\n" );
	Site.ReceiveData( NULL );
	/*Site.SendCommand( "PWD\r\n" );
	Site.ReceiveData( );
	Site.SendCommand( "HELP\r\n" );
	Site.ReceiveData( );
	Site.SendCommand( "LIST\r\n" );
	Site.ReceiveData( );*/
	std::string *pDirectory = NULL;
	Site.ListCurrentDirectory( pDirectory );
	Site.SendCommand( "QUIT\r\n" );
	Site.ReceiveData( NULL );
	Site.Disconnect( );

	getch( );

	return 0;
}
