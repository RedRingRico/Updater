#include <Commands.hpp>
#include <Windows.h>
#include <string>
#include <cstring>

// This may be a terrible idea
const char *g_pCommands[ 30 ] =
{
	// Long name				Short name	Command Name
	"config-file",				"f",		"ConfigFile",
	"site",						"s",		"Site",
	"port",						"p",		"Port",
	"project",					"r",		"Project",
	"platform",					"m",		"Platform",
	"build-type",				"t",		"BuildType",
	"output-download-summary",	"o",		"DownloadSummary",
	"active-mode",				"a",		"ActiveMode",
	"port-range",				"n",		"PortRange",
	"usage",					"u",		"Usage"
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
			if( strcmp( pParameter, g_pCommands[ ( i*3 ) ] ) == 0 )
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

		size_t CommandLen = strlen( g_pCommands[ ( Command*3 )+2 ] );
		( *p_ppParameter ) = new char[ CommandLen+1 ];
		strncpy( ( *p_ppParameter ),g_pCommands[ ( Command*3 )+2 ], CommandLen );
		( *p_ppParameter )[ CommandLen ] = '\0';

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

namespace Updater
{
	Command::Command( )
	{
	}

	Command::~Command( )
	{
	}

	int Command::Add( const std::string &p_FunctionName,
		const std::string &p_CommandName, const std::string &p_ShortCmdName,
		const COMMAND_FUNCTION p_Function,
		const size_t p_ParamCount, const PARAM_TYPE *p_pParamList )
	{
		// Make sure the function isn't already defined
		std::list< COMMAND >::iterator CmdItr = m_Commands.begin( );

		for( ; CmdItr != m_Commands.end( ); ++CmdItr )
		{
			if( ( *CmdItr ).FunctionName.compare( p_FunctionName ) == 0 )
			{
				return 1;
			}
			if( ( *CmdItr ).CommandName.compare( p_CommandName ) == 0 )
			{
				return 1;
			}
			if( ( *CmdItr ).ShortCmdName.compare( p_ShortCmdName ) == 0 )
			{
				return 1;
			}
		}

		COMMAND TmpCmd;
		memset( &TmpCmd, 0, sizeof( COMMAND ) );

		TmpCmd.Function = p_Function;
		TmpCmd.ParamCount = p_ParamCount;
		TmpCmd.pParams = const_cast< PARAM_TYPE* >( p_pParamList );
		TmpCmd.FunctionName = p_FunctionName;
		TmpCmd.CommandName = p_CommandName;
		TmpCmd.ShortCmdName = p_ShortCmdName;

		m_Commands.push_back( TmpCmd ); 

		return 0;
	}

	void Command::GetCommandParamCount( std::string p_Command, int *p_pCount )
	{
		CommandItr CmdItr = m_Commands.begin( );

		for( ; CmdItr != m_Commands.end( ); ++CmdItr )
		{
			if( ( *CmdItr ).CommandName.compare( p_Command ) == 0 )
			{
				*p_pCount = ( *CmdItr ).ParamCount;
				return;
			}
			if( ( *CmdItr ).FunctionName.compare( p_Command ) == 0 )
			{
				*p_pCount = ( *CmdItr ).ParamCount;
				return;
			}
			if( ( *CmdItr ).ShortCmdName.compare( p_Command ) == 0 )
			{
				*p_pCount = ( *CmdItr ).ParamCount;
				return;
			}
		}

		*p_pCount = -1;
	}

	void Command::GetCommandList( std::list< std::string > &p_Commands )
	{
		std::list< COMMAND >::iterator Itr = m_Commands.begin( );

		for( ; Itr != m_Commands.end( ); ++Itr )
		{
			p_Commands.push_front( ( *Itr ).FunctionName );
		}
	}

	int Command::Execute( const char *p_pCommandName,
		const char *p_pParameters )
	{
		std::list< COMMAND >::iterator Itr = m_Commands.begin( );
		for( ; Itr != m_Commands.end( ); ++Itr )
		{
			if( ( ( *Itr ).FunctionName.compare( p_pCommandName ) == 0 ) ||
				( ( *Itr ).CommandName.compare( p_pCommandName ) == 0 ) ||
				( ( *Itr ).ShortCmdName.compare( p_pCommandName ) == 0 ) )
			{
				int Result = 
					( *Itr ).Function( p_pParameters );
				return Result;
			}
		}

		return 1;
	}
}
