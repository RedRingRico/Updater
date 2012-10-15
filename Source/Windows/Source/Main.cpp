#include <iostream>
#include <Log.hpp>

#include <conio.h>
int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Updater Version 0.0.0.1 [Prototype]" << std::endl;           
	Log FTPLog;
	FTPLog.SetLogFile( "testlog.log", true );
	FTPLog.Print( "MORE OLD TEST!\n" );
	getch( );
	return 0;
}
