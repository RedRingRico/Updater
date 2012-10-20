#include <WinSock2.h>
#include <iostream>
#include <Log.hpp>
#include <FTPSite.hpp>
#include <conio.h>
#include <Version.h>
#include <ConfigFile.hpp>
#include <Main.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Updater Version " << VERSION_MAJOR << "." <<
		VERSION_MINOR << "." << VERSION_REVISION << "." <<
		VERSION_BUILD << ( HG_LOCAL_MODIFICATIONS ? "M" : "" ) << std::endl;

	if( SetupCommands( ) != 0 )
	{
		return 0;
	}

	// Set the global configuration options to their default values
	g_Site.clear( );
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
				g_Site = Value;
			}
			if( Configuration.GetValue( "port", Value ) == 0 )
			{
				g_Port = Value;
			}
			if( Configuration.GetValue( "active", Value ) == 0 )
			{
				size_t i = Value.compare( "false" );
				if( Value.compare( "true" ) == 0 )
				{
					g_ActiveMode = true;
				}
				else if( Value.compare( "false" ) == 0 )
				{
					g_ActiveMode = false;
				}
			}
			if( Configuration.GetValue( "port_range", Value ) == 0 )
			{
				// Get the delimited values, then try to convert them to
				// their integer values
				std::string Port1, Port2;
				size_t Delimiter = 0;
				Delimiter = Value.find_first_of( ":" );
				if( Delimiter != std::string::npos )
				{
					Port1 = Value.substr( 0, Delimiter );
					Port2 = Value.substr( Delimiter+1, Value.size( ) );
					int PortA, PortB;
					PortA = atoi( Port1.c_str( ) );
					PortB = atoi( Port2.c_str( ) );

					g_PortRange.Port[ 0 ] = PortA;
					g_PortRange.Port[ 1 ] = PortB;
				}
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

	std::string Value;
	int PortRange[ 2 ] = { 40000, 40010 };
	g_LineCommands.Execute( "Site", g_Site.c_str( ) );
	g_LineCommands.Execute( "PortRange", PortRange );
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
	g_LineCommands.Add( "Usage", DisplayUsage, 0, NULL );
	PARAM_TYPE SiteParams = PARAM_STRING;
	PARAM_TYPE PortParams [ 2 ] = { PARAM_INTEGER, PARAM_INTEGER };
	g_LineCommands.Add( "Site", SetSite, 1, &SiteParams );
	g_LineCommands.Add( "PortRange", SetPortRange, 2, PortParams );

	return 0;
}

void CleanupGlobals( )
{
}
