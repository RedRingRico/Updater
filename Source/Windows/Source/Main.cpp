#include <WinSock2.h>
#include <Log.hpp>
#include <FTPSite.hpp>
#include <Version.h>
#include <ConfigFile.hpp>
#include <Main.hpp>
#include <ProgramCommands.hpp>
#include <Utility.hpp>
#include <CursesHelper.hpp>
#include <curses.h>
#include <sstream>
#include <iostream>

int main( int p_Argc, char **p_ppArgv )
{
	// Don't continue if CURSES is fubar
	if( StartCURSES( ) != 0 )
	{
		std::cout << "[ERROR] Could not start PDCURSES" << std::endl <<
			std::endl;
		return 1;
	}

	int ScreenSize[ 2 ] = { 0, 0 };
	getmaxyx( stdscr, ScreenSize[ 1 ], ScreenSize[ 0 ] );

	std::stringstream Title;
	Title << "Updater Version " << VERSION_MAJOR << "." << VERSION_MINOR <<
		"." << VERSION_REVISION << "." << VERSION_BUILD << "\n";
		
	attron( A_BOLD );
	attron( COLOR_PAIR( 1 ) );
	mvprintw( 0, ( ScreenSize[ 0 ] / 2 )-( Title.str( ).size( ) / 2 ), "%s\n",
		Title.str( ).c_str( ) );
	attroff( COLOR_PAIR( 1 ) );
	attroff( A_BOLD );
	
	Updater::ProgramCommands Commands;

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
		Print( MSG_WARNING, stdscr, "Could not find file %s\n",
			ConfigurationPath.c_str( ) );
		refresh( );
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
			//char *pCommand = NULL;
			// Is this a valid command line parameter?
			if( Commands.ValidCommand( Arg ) == 0 )
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

				i += ParamCount;
			}
		}
	}

	printw( "Configuration information\n\n" );
	printw( "Server name: %s\n", Commands.GetSite( ) );
	printw( "Server port: %s\n", Commands.GetPort( ) );
	refresh( );
	if( Commands.GetActiveMode( ) )
	{
		printw( "Active mode\n" );
		int *pPorts = new int[ 2 ];
		Commands.GetPortRange( &pPorts );
		printw( "\tPort range: %d.%d\n", pPorts[ 0 ], pPorts[ 1 ] );
		SAFE_DELETE_ARRAY( pPorts );
	}
	else
	{
		printw( "Passive mode\n" );;
	}
	if( strlen( Commands.GetProject( ) ) != 0 )
	{
		printw( "Project: %s\n", Commands.GetProject( ) );
	}
	if( strlen( Commands.GetPlatform( ) ) != 0 )
	{
		printw( "Platform: %s\n", Commands.GetPlatform( ) );
	}
	if( strlen( Commands.GetBuildType( ) ) != 0 )
	{
		printw( "Build Type: %s\n", Commands.GetBuildType( ) );
	}
	if( strlen( Commands.GetDownloadSummary( ) ) != 0)
	{
		printw( "Download Summary File: %s\n",
			Commands.GetDownloadSummary( ) );
	}
	printw( "\n" );
	refresh( );
	
	FTPSite Site;

	if( Site.Initialise( Commands.GetActiveMode( ) ) != 0 )
	{
		Print( MSG_ERROR, stdscr,
			"Failed to initilaise FTP server settings\n" );
		StopCURSES( );
		return 1;
	}

	Print( MSG_NORMAL, stdscr, "Connecting to: %s", Commands.GetSite( ) );
	Site.SetAddress( Commands.GetSite( ) );
	if( Site.Connect( ) != 0 )
	{
		Print( MSG_ERROR, stdscr, "\nFailed to connect to: %s\n",
			Commands.GetSite( ) );

		StopCURSES( );
		return 1;
	}

	int CurY = 0;
	int CurX = 0;
	getyx( stdscr, CurY, CurX );
	MVPrint( CurY, ScreenSize[ 0 ]-strlen( "[OK]" ), MSG_INFO,
		stdscr, "[OK]" );

	std::list< std::string > Directory;
	Site.ListCurrentDirectory( Directory );

	if( Directory.size( ) > 0 )
	{
		std::list< std::string >::iterator Itr = Directory.begin( );
		Print( MSG_NORMAL, stdscr, "Directory contents\n" );

		for( ; Itr != Directory.end( ); ++Itr )
		{
			Print( MSG_NORMAL, stdscr, "\t>%s\n", ( *Itr ).c_str( ) );
		}
	}

//	Site.DownloadFile( "projects/braindead/builds/windows/x86_32/latest/BrainDead_4.6.exe" );
	
	Site.Disconnect( );

	StopCURSES( );

	return 0;
}
