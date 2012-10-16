#include <WinSock2.h>
#include <iostream>
#include <Log.hpp>
#include <FTPSite.hpp>
#include <conio.h>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Updater Version 0.0.0.1 [Prototype]" << std::endl;           
	Log FTPLog;
	FTPLog.SetLogFile( "testlog.log", true );
	FTPLog.Print( "MORE OLD TEST!\n" );

	FTPSite Site;

	Site.Initialise( );
	Site.Connect( );
	Site.Disconnect( );
	getch( );
	return 0;
}
