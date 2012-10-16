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

	Site.ReceiveData( );
	Site.SendCommand( "USER anonymous" );
	Site.ReceiveData( );

	Site.Disconnect( );

	getch( );

	return 0;
}
