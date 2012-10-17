#include <WinSock2.h>
#include <iostream>
#include <Log.hpp>
#include <FTPSite.hpp>
#include <conio.h>
#include <Version.h>
#include <Commands.hpp>

#define SAFE_DELETE_ARRAY( arr ) \
	{\
	if( arr != NULL )\
	{\
		delete [ ]arr;\
		arr = NULL;\
	}\
	}

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Updater Version " << VERSION_MAJOR << "." <<
		VERSION_MINOR << "." << VERSION_REVISION << "." <<
		VERSION_BUILD << ( HG_LOCAL_MODIFICATIONS ? "M" : "" ) << std::endl;

	std::string ConfigurationFile;
	bool AlternateConfiguration = false;
	bool ActiveMode = false;

	if( p_Argc > 0 )
	{
		for( size_t i = 1; i < p_Argc; ++i )
		{
			char *pCommand = NULL, *pParam = NULL;
			int Command = GetCommandLineParameter( p_ppArgv[ i ], &pCommand );

			if( Command == INVALID_COMMAND )
			{
				SAFE_DELETE_ARRAY( pCommand );
				DisplayUsage( );
				return 1;
			}

			SAFE_DELETE_ARRAY( pCommand );

			// Active mode and Usage commands do not accept parameters
			if( Command == COMMAND_ACTIVE_MODE )
			{
				ActiveMode = true;
				continue;
			}
			if( Command == COMMAND_USAGE )
			{
				DisplayUsage( );
				continue;
			}

			// Process commands with a parameter
			if( p_Argc < i+1 )
			{
				continue;
			}

			// Jump to the next argument
			++i;
			int ReturnStatus = 0;

			switch( Command )
			{
			// Configuration file
			case COMMAND_CONFIG_FILE:
				{
					ReturnStatus = GetConfigFile( p_ppArgv[ i ], &pParam );
					if( ReturnStatus != 0 )
					{
						break;
					}

					ConfigurationFile.clear( );
					ConfigurationFile.append( pParam );
					AlternateConfiguration = true;

					break;
				}
			default:
				{
					ReturnStatus = 1;
					break;
				}
			}

			if( ReturnStatus != 0 )
			{
				DisplayUsage( );
				SAFE_DELETE_ARRAY( pCommand );
				SAFE_DELETE_ARRAY( pParam );

				return 1;
			}

			SAFE_DELETE_ARRAY( pCommand );
			SAFE_DELETE_ARRAY( pParam );
		}
	}

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
