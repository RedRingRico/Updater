#include <WinSock2.h>
#include <iostream>
#include <Log.hpp>
#include <FTPSite.hpp>
#include <conio.h>
#include <Version.h>
#include <ConfigFile.hpp>
#include <Main.hpp>
#include <ProgramCommands.hpp>


int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Updater Version " << VERSION_MAJOR << "." <<
		VERSION_MINOR << "." << VERSION_REVISION << "." <<
		VERSION_BUILD << ( HG_LOCAL_MODIFICATIONS ? "M" : "" ) << std::endl;

	if( SetupCommands( ) != 0 )
	{
		return 0;
	}
	Updater::ProgramCommands TestCmds;

	TestCmds.Initialise( );
	int ParamCount = 0;
	TestCmds.GetCommandParamCount( "n", &ParamCount );
	TestCmds.Execute( "n", "1024:10000" );

	// Set the global configuration options to their default values
/*	g_Site.clear( );
	g_Port.clear( );
	g_Project.clear( );
	g_Platform.clear( );
	g_BuildType.clear( );
	g_DownloadSummary.clear( );

	g_Site.append( "opengamedevelopers.org" );
	g_Port.append( "21" );

	g_PortRange.Port[ 0 ] = 40000;
	g_PortRange.Port[ 1 ] = 40010;
	
	g_ActiveMode = false;

	std::string ConfigurationPath;
	ConfigurationPath.clear( );

	ConfigFile Configuration;

	if( ConfigurationPath.empty( ) )
	{
		Configuration.SetConfigFile( "default.cfg" );
		if( Configuration.Parse( ) != 0 )
		{
			std::cout << "Could not find file default.cfg" << std::endl;
		}
		else
		{
			Configuration.Parse( );
			std::string Value;
			Value.clear( );

			// Try and get the values, if none exist, no problem
			if( Configuration.GetValue( "site", Value ) == 0 )
			{
				g_LineCommands.Execute( "Site", Value.c_str( ) );
			}
			if( Configuration.GetValue( "port", Value ) == 0 )
			{
				g_Port = Value;
			}
			if( Configuration.GetValue( "active", Value ) == 0 )
			{
				g_LineCommands.Execute( "ActiveMode", Value.c_str( ) );
			}
			if( Configuration.GetValue( "port_range", Value ) == 0 )
			{
				g_LineCommands.Execute( "PortRange", Value.c_str( ) );
			}
			if( Configuration.GetValue( "project", Value ) == 0 )
			{
				g_Project = Value;
			}
			if( Configuration.GetValue( "platform", Value ) == 0 )
			{
				g_Platform = Value;
			}
			if( Configuration.GetValue( "build_type", Value ) == 0 )
			{
				g_BuildType = Value;
			}
			if( Configuration.GetValue( "download_summary", Value ) == 0 )
			{
				g_DownloadSummary = Value;
			}
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
				g_LineCommands.Execute( pCommand, p_ppArgv[ i+1 ] );
			}
		}
	}

	std::cout << "Configuration information" << std::endl << std::endl;
	std::cout << "Server name: " << g_Site << std::endl;
	std::cout << "Server port: " << g_Port << std::endl;
	if( g_ActiveMode )
	{
		std::cout << "Active mode" << std::endl;
		std::cout << "\tPort range: " << g_PortRange.Port[ 0 ] <<
			":" << g_PortRange.Port[ 1 ] << std::endl;
	}
	else
	{
		std::cout << "Passive mode" << std::endl;
	}
	if( !g_Project.empty( ) )
	{
		std::cout << "Project: " << g_Project << std::endl;
	}
	if( !g_Platform.empty( ) )
	{
		std::cout << "Platform: "<< g_Platform << std::endl;
	}
	if( !g_BuildType.empty( ) )
	{
		std::cout << "Build Type: " << g_BuildType << std::endl;
	}
	if( !g_DownloadSummary.empty( ) )
	{
		std::cout << "Download Summary File: " << g_DownloadSummary <<
			std::endl << std::endl;
	}
	*/
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

	CleanupGlobals( );

	return 0;
}

int SetupCommands( )
{
	PARAM_TYPE OneString = PARAM_STRING;
	PARAM_TYPE OneBool = PARAM_BOOLEAN;
	PARAM_TYPE PortRange [ 2 ] = { PARAM_INTEGER, PARAM_INTEGER };
/*	g_LineCommands.Add( "Usage", Updater::DisplayUsage, 0, NULL );
	g_LineCommands.Add( "Site",  Updater::SetSite, 1, &OneString );
	g_LineCommands.Add( "Port",  Updater::SetPortCmd, 1, &OneString );
	g_LineCommands.Add( "ActiveMode",  Updater::SetActiveMode, 1, &OneBool );
	g_LineCommands.Add( "PortRange",  Updater::SetPortRange, 2, PortRange );
	g_LineCommands.Add( "Project",  Updater::SetProject, 1, &OneString );
	g_LineCommands.Add( "Platform",  Updater::SetPlatform, 1, &OneString );
	g_LineCommands.Add( "BuildType",  Updater::SetBuildType, 1, &OneString );
	g_LineCommands.Add( "DownloadSummary",  Updater::SetDownloadSummary, 1,
		&OneString );
		*/
	return 0;
}

void CleanupGlobals( )
{
}
