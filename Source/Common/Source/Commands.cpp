#include <Commands.hpp>
#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif
#include <string>
#include <cstring>

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

	int Command::ValidCommand( const std::string &p_Command )
	{
		CommandItr Itr = m_Commands.begin( );

		for( ; Itr != m_Commands.end( ); ++Itr )
		{
			if( ( ( *Itr ).CommandName.compare( p_Command ) == 0 ) ||
				( ( *Itr ).FunctionName.compare( p_Command ) == 0 ) ||
				( ( *Itr ).ShortCmdName.compare( p_Command ) == 0 ) )
			{
				return 0;
			}
		}
		return 1;
	}

	int Command::Execute( const char *p_pCommandName,
		const char **p_pParameters )
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
