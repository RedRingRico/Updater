#include <Commands.hpp>
#include <Windows.h>
#include <string>
#include <cstring>

// This may be a terrible idea
const char *g_pCommands[ 20 ] =
{
	// Long name				Short name
	"config-file",				"f",
	"site",						"s",
	"port",						"p",
	"project",					"r",
	"platform",					"m",
	"build-type",				"t",
	"output-download-summary",	"o",
	"active-mode",				"a",
	"port-range",				"n",
	"usage",					"u"
};

int GetCommandLineParameter( const char *p_pArg, char **p_ppParameter )
{
	if( p_pArg == NULL || strlen( p_pArg ) == 0 )
	{
		return 1;
	}

	if( strlen( p_pArg ) < 2 )
	{
		return 1;
	}
	const char pTacks[ 3 ] = { p_pArg[ 0 ], p_pArg[ 1 ], '\0' };
	const char pTack[ 2 ] = { p_pArg[ 0 ], '\0' };
	// Parameters must begin with a hypen or double-hyphen
	if( strcmp( pTacks, "--" ) == 0 )
	{
		if( strlen( p_pArg ) <= 2 )
		{
			return 1;
		}
		char *pParameter = new char[ strlen( p_pArg )-1 ];
		if( pParameter == NULL )
		{
			return 1;
		}
		strncpy( pParameter, p_pArg+2, strlen( p_pArg )-2 );
		pParameter[ strlen( p_pArg )-2 ] = '\0';

		size_t Command = MAX_COMMANDS+1;

		for( size_t i = 0; i < MAX_COMMANDS; ++i )
		{
			if( strcmp( pParameter, g_pCommands[ ( i*2 ) ] ) == 0 )
			{
				Command = i;
				break;
			}
		}
		if( Command == MAX_COMMANDS+1 )
		{
			delete [ ] pParameter;
			pParameter = NULL;
			return INVALID_COMMAND;
		}

		( *p_ppParameter ) = new char[ strlen( pParameter )+1 ];
		strncpy( ( *p_ppParameter ), pParameter, strlen( pParameter ) );;
		( *p_ppParameter )[ strlen( pParameter ) ] = '\0';

		delete [ ] pParameter;
		pParameter = NULL;

		return Command;
	}
	else if( strcmp( &pTacks[ 0 ], "-" ) == 0 )
	{
	}
	else
	{
		return INVALID_COMMAND;
	}

	return INVALID_COMMAND;
}

int GetConfigFile( const char *p_pArg, char **p_pConfigFile )
{
	if( p_pArg == NULL )
	{
		return 1;
	}
	// Make sure no illegal characters are in the configuration file
	std::string ConfigFile( p_pArg );

	if( ConfigFile.find( ":|?" ) != std::string::npos )
	{
		return 1;
	}

	( *p_pConfigFile ) = new char [ ConfigFile.size( ) + 1 ];
	strncpy( ( *p_pConfigFile ), ConfigFile.c_str( ), ConfigFile.size( ) );
	( *p_pConfigFile )[ ConfigFile.size( ) ] = '\0';

	return 0;
}

void DisplayUsage( )
{
}
