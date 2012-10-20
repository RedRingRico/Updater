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


Commands::Commands( )
{
}

Commands::~Commands( )
{
}

int Commands::Add( const std::string &p_Name, const COMMAND_FUNCTION p_Function,
	const size_t p_ParamCount, const PARAM_TYPE *p_pParamList )
{
	// Make sure the function isn't already defined
	std::list< COMMAND >::iterator CmdItr = m_Commands.begin( );

	for( ; CmdItr != m_Commands.end( ); ++CmdItr )
	{
		if( ( *CmdItr ).Name.compare( p_Name ) == 0 )
		{
			return 1;
		}
	}
	COMMAND TmpCmd;
	memset( &TmpCmd, 0, sizeof( COMMAND ) );

	TmpCmd.Function = p_Function;
	TmpCmd.ParamCount = p_ParamCount;
	TmpCmd.pParams = const_cast< PARAM_TYPE* >( p_pParamList );
	TmpCmd.Name = p_Name;

	m_Commands.push_back( TmpCmd ); 

	return 0;
}

void Commands::GetCommandList( std::list< std::string > &p_Commands )
{
	std::list< COMMAND >::iterator Itr = m_Commands.begin( );

	for( ; Itr != m_Commands.end( ); ++Itr )
	{
		p_Commands.push_front( ( *Itr ).Name );
	}
}

int Commands::Execute( const char *p_pCommandName, const void *p_pParameters )
{
	std::list< COMMAND >::iterator Itr = m_Commands.begin( );
	for( ; Itr != m_Commands.end( ); ++Itr )
	{
		if( ( *Itr ).Name.compare( p_pCommandName ) == 0 )
		{
			int Result = 
				( *Itr ).Function( const_cast< void * >( p_pParameters ) );
			return Result;
		}
	}

	return 1;
}
