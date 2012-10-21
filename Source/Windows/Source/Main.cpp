#include <WinSock2.h>
#include <iostream>
#include <Log.hpp>
#include <FTPSite.hpp>
#include <Version.h>
#include <ConfigFile.hpp>
#include <Main.hpp>
#include <ProgramCommands.hpp>
#include <Utility.hpp>
#include <CursesHelper.hpp>
#include <curses.h>


int main( int p_Argc, char **p_ppArgv )
{
	StartCURSES( );

	printw( "Updater | Version %d.%d.%d.%d%s\n", VERSION_MAJOR, VERSION_MINOR,
		VERSION_REVISION, VERSION_BUILD, HG_LOCAL_MODIFICATIONS ? "M" : "" );

	Updater::ProgramCommands Commands;
	
	StopCURSES( );

	Commands.Initialise( );

	// Get the configuration file from the command line
	if( p_Argc > 1 )
	{
		for( size_t i = 1; i < p_Argc; ++i )
		{
			std::string Arg = p_ppArgv[ i ];
			char *pCommand = NULL;
			// Is this a valid command line parameter?
			if( ( Arg.compare( "--config-file" ) == 0 ) ||
				( Arg.compare( "-f" ) == 0 ) )
			{
				int ParamCount = 0;
				Commands.GetCommandParamCount( Arg.c_str( ), &ParamCount );
				char **ppParams = NULL;

				if( ParamCount > 0 )
				{
					ppParams = new char*[ ParamCount ];
					for( int j = 1; j < ParamCount+1; ++j )
					{
						if( j+i == p_Argc )
						{
							for( size_t t = 0; t < j; ++t )
							{
								SAFE_DELETE_ARRAY( ppParams[ j ] );
							}

							SAFE_DELETE_ARRAY( ppParams );
							break;
						}
						int ParamPos = j-1;
						size_t ArgLen = strlen( p_ppArgv[ i+j ] );
						ppParams[ ParamPos ] = new char[ ArgLen+1 ];
						strncpy( ppParams[ ParamPos ], p_ppArgv[ i+j ],
							ArgLen );
						ppParams[ ParamPos ][ ArgLen ] = '\0';
					}
				}
				else
				{
					// Make sure that ppParams[ n ] doesn't get deleted as it
					// doesn't exist
					ParamCount = 0;
				}

				Commands.Execute( Arg.c_str( ),
					const_cast< const char ** >( ppParams ) );

				for( size_t j = 0; j < ParamCount; ++j )
				{
					SAFE_DELETE_ARRAY( ppParams[ j ] );
				}

				SAFE_DELETE_ARRAY( ppParams );

				// Done, don't load another configuration file
				break;
			}
		}
	}

	std::string ConfigurationPath;
	ConfigurationPath.clear( );

	ConfigFile Configuration;

	if( strlen( Commands.GetConfigurationFile( ) ) != 0 )
	{
		ConfigurationPath.append( Commands.GetConfigurationFile( ) );
	}

	if( ConfigurationPath.empty( ) )
	{
		Configuration.SetConfigFile( "default.cfg" );
	}
	else
	{
		Configuration.SetConfigFile( ConfigurationPath.c_str( ) );
	}

	if( Configuration.Parse( ) != 0 )
	{
		std::cout << "Could not find file " << ConfigurationPath << std::endl;
	}
	else
	{
		Configuration.Parse( );
		std::string Value;
		Value.clear( );

		// CLEANUP!
		// The code below is repeated an awful lot.  It would be better to
		// procedurally get the value from the configuration file, then
		// use the value in the Commands.Execute function
		// !CLEANUP

		// Try and get the values, if none exist, no problem
		if( Configuration.GetValue( "site", Value ) == 0 )
		{
			char *pValue = new char[ Value.size( )+1 ];
			strncpy( pValue, Value.c_str( ), Value.size( ) );
			pValue[ Value.size( ) ] = '\0';
			Commands.Execute( "Site", const_cast< const char ** >( &pValue ) );
			SAFE_DELETE_ARRAY( pValue );
		}
		if( Configuration.GetValue( "port", Value ) == 0 )
		{
			char *pValue = new char[ Value.size( )+1 ];
			strncpy( pValue, Value.c_str( ), Value.size( ) );
			pValue[ Value.size( ) ] = '\0';
			Commands.Execute( "Port", const_cast< const char ** >( &pValue ) );
			SAFE_DELETE_ARRAY( pValue );
		}
		if( Configuration.GetValue( "active", Value ) == 0 )
		{
			char *pValue = new char[ Value.size( )+1 ];
			strncpy( pValue, Value.c_str( ), Value.size( ) );
			pValue[ Value.size( ) ] = '\0';
			Commands.Execute( "ActiveMode",
				const_cast< const char ** >( &pValue ) );
			SAFE_DELETE_ARRAY( pValue );
		}
		if( Configuration.GetValue( "port_range", Value ) == 0 )
		{
			char *pValue = new char[ Value.size( )+1 ];
			strncpy( pValue, Value.c_str( ), Value.size( ) );
			pValue[ Value.size( ) ] = '\0';
			Commands.Execute( "PortRange",
				const_cast< const char ** >( &pValue ) );
			SAFE_DELETE_ARRAY( pValue );
		}
		if( Configuration.GetValue( "project", Value ) == 0 )
		{
			char *pValue = new char[ Value.size( )+1 ];
			strncpy( pValue, Value.c_str( ), Value.size( ) );
			pValue[ Value.size( ) ] = '\0';
			Commands.Execute( "Project",
				const_cast< const char ** >( &pValue ) );
			SAFE_DELETE_ARRAY( pValue );
		}
		if( Configuration.GetValue( "platform", Value ) == 0 )
		{
			char *pValue = new char[ Value.size( )+1 ];
			strncpy( pValue, Value.c_str( ), Value.size( ) );
			pValue[ Value.size( ) ] = '\0';
			Commands.Execute( "Platform",
				const_cast< const char ** >( &pValue ) );
			SAFE_DELETE_ARRAY( pValue );
		}
		if( Configuration.GetValue( "build_type", Value ) == 0 )
		{
			char *pValue = new char[ Value.size( )+1 ];
			strncpy( pValue, Value.c_str( ), Value.size( ) );
			pValue[ Value.size( ) ] = '\0';
			Commands.Execute( "BuildType",
				const_cast< const char ** >( &pValue ) );
			SAFE_DELETE_ARRAY( pValue );
		}
		if( Configuration.GetValue( "download_summary", Value ) == 0 )
		{
			char *pValue = new char[ Value.size( )+1 ];
			strncpy( pValue, Value.c_str( ), Value.size( ) );
			pValue[ Value.size( ) ] = '\0';
			Commands.Execute( "DownloadSummary",
				const_cast< const char ** >( &pValue ) );
			SAFE_DELETE_ARRAY( pValue );
		}
	}
	
	// Read any parameters from the command line and execute them
	if( p_Argc > 1 )
	{
		for( size_t i = 1; i < p_Argc; ++i )
		{
			std::string Arg = p_ppArgv[ i ];
			char *pCommand = NULL;
			// Is this a valid command line parameter?
			if( GetCommandLineParameter( Arg.c_str( ), &pCommand )
				!= INVALID_COMMAND )
			{
				int ParamCount = 0;
				Commands.GetCommandParamCount( Arg.c_str( ), &ParamCount );
				char **ppParams = NULL;

				if( ParamCount > 0 )
				{
					ppParams = new char*[ ParamCount ];
					for( int j = 1; j < ParamCount+1; ++j )
					{
						if( j+i == p_Argc )
						{
							for( size_t t = 0; t < j; ++t )
							{
								SAFE_DELETE_ARRAY( ppParams[ j ] );
							}

							SAFE_DELETE_ARRAY( ppParams );
							break;
						}
						int ParamPos = j-1;
						size_t ArgLen = strlen( p_ppArgv[ i+j ] );
						ppParams[ ParamPos ] = new char[ ArgLen+1 ];
						strncpy( ppParams[ ParamPos ], p_ppArgv[ i+j ],
							ArgLen );
						ppParams[ ParamPos ][ ArgLen ] = '\0';
					}
				}
				else
				{
					// Make sure that ppParams[ n ] doesn't get deleted as it
					// doesn't exist
					ParamCount = 0;
				}

				Commands.Execute( pCommand,
					const_cast< const char ** >( ppParams ) );

				for( size_t j = 0; j < ParamCount; ++j )
				{
					SAFE_DELETE_ARRAY( ppParams[ j ] );
				}

				SAFE_DELETE_ARRAY( ppParams );

				i += ParamCount;
			}
		}
	}

	std::cout << "Configuration information" << std::endl << std::endl;
	std::cout << "Server name: " << Commands.GetSite( ) << std::endl;
	std::cout << "Server port: " << Commands.GetPort( ) << std::endl;
	if( Commands.GetActiveMode( ) )
	{
		std::cout << "Active mode" << std::endl;
		int *pPorts = new int[ 2 ];
		Commands.GetPortRange( &pPorts );
		std::cout << "\tPort range: " << pPorts[ 0 ] <<
			":" << pPorts[ 1 ] << std::endl;
		SAFE_DELETE_ARRAY( pPorts );
	}
	else
	{
		std::cout << "Passive mode" << std::endl;
	}
	if( strlen( Commands.GetProject( ) ) != 0 )
	{
		std::cout << "Project: " << Commands.GetProject( ) << std::endl;
	}
	if( strlen( Commands.GetPlatform( ) ) != 0 )
	{
		std::cout << "Platform: "<< Commands.GetPlatform( ) << std::endl;
	}
	if( strlen( Commands.GetBuildType( ) ) != 0 )
	{
		std::cout << "Build Type: " << Commands.GetBuildType( ) << std::endl;
	}
	if( strlen( Commands.GetDownloadSummary( ) ) != 0)
	{
		std::cout << "Download Summary File: " << Commands.GetDownloadSummary( )
			<< std::endl << std::endl;
	}
	
	/*
	FTPSite Site;

	if( Site.Initialise( ) != 0 )
	{
		CleanupGlobals( );
		return 1;
	}
	if( Site.Connect( g_Site.c_str( ) ) != 0 )
	{
		CleanupGlobals( );
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
*/
	std::cout << "Press any key to exit" << std::endl;
	getch( );

	return 0;
}
