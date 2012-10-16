#include <WinSock2.h>
#include <iostream>
#include <Log.hpp>
#include <FTPSite.hpp>
#include <conio.h>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Updater Version 0.0.0.8 [Prototype]" << std::endl;           

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
	Site.SendCommand( "FEAT\r\n" );
	Site.ReceiveData( NULL );
	Site.SendCommand( "TYPE I\r\n" );
	Site.ReceiveData( NULL );
	std::string *pDirectory = NULL;
	Site.ListCurrentDirectory( pDirectory );
	Site.SendCommand( "QUIT\r\n" );
	Site.ReceiveData( NULL );
	Site.Disconnect( );

	std::cout << "Press any key to exit" << std::endl;
	getch( );

	return 0;
}
