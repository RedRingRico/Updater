#include <ProgramCommands.hpp>
#include <iostream>

namespace Updater
{
	ProgramCommands::ProgramCommands( )
	{
	}

	ProgramCommands::~ProgramCommands( )
	{
	}

	int ProgramCommands::Add( const std::string &p_FunctionName,
			const std::string &p_CommandName, const std::string &p_ShortCmdName,
			const CLASS_COMMAND_FUNCTION p_Function,
			const size_t p_ParamCount, const PARAM_TYPE *p_pParamList,
			ProgramCommands *p_pSelf )
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

		CLASS_COMMAND TmpCmd;
		memset( &TmpCmd, 0, sizeof( COMMAND ) );

		TmpCmd.Function = p_Function;
		TmpCmd.ParamCount = p_ParamCount;
		TmpCmd.pParams = const_cast< PARAM_TYPE* >( p_pParamList );
		TmpCmd.FunctionName = p_FunctionName;
		TmpCmd.CommandName = p_CommandName;
		TmpCmd.ShortCmdName = p_ShortCmdName;

		m_ClassCommands.push_back( TmpCmd ); 
		return 0;
	}

	int ProgramCommands::Initialise( )
	{
		PARAM_TYPE OneString = PARAM_STRING;
		PARAM_TYPE OneInt = PARAM_INTEGER;

		//m_SetPort = ( CLASS_COMMAND_FUNCTION )&(SetPort);

		this->Add( "Usage", "usage", "u", DisplayUsage, 0, NULL, this );
		this->Add( "Site", "site", "s", SetSite, 1, &OneString, this );
		this->Add( "Port", "port", "p", SetPort, 1, &OneString, this );
		this->Add( "ActiveMode", "active-mode", "a", SetActiveMode, 0, 0,
			this );
		this->Add( "PortRange", "port-range", "n",  SetPortRange, 1, &OneInt,
			this );
		this->Add( "Project", "project", "r", SetProject, 1, &OneString, this );
		this->Add( "Platform", "platform", "m", SetPlatform, 1, &OneString,
			this );
		this->Add( "BuildType", "build-type", "t", SetBuildType, 1, &OneString,
			this);
		this->Add( "DownloadSummary", "output-download-summary", "o",
			SetDownloadSummary, 1, &OneString, this );
		return 0;
	}

	int ProgramCommands::Execute( const char *p_pCommandName,
		const char *p_pParameters )
	{
		std::list< CLASS_COMMAND >::iterator Itr = m_ClassCommands.begin( );
		for( ; Itr != m_ClassCommands.end( ); ++Itr )
		{
			if( ( ( *Itr ).FunctionName.compare( p_pCommandName ) == 0 ) ||
				( ( *Itr ).CommandName.compare( p_pCommandName ) == 0 ) ||
				( ( *Itr ).ShortCmdName.compare( p_pCommandName ) == 0 ) )
			{
				int Result = 
					( *Itr ).Function( p_pParameters, this );
				return Result;
			}
		}

		return Updater::Command::Execute( p_pCommandName, p_pParameters );

		return 1;
	}

	void ProgramCommands::GetCommandParamCount( std::string p_Command,
		int *p_pCount )
	{
		ClassCmdItr Itr = m_ClassCommands.begin( );

		for( ; Itr != m_ClassCommands.end( ); ++Itr )
		{
			if( ( *Itr ).FunctionName.compare( p_Command ) == 0 )
			{
				*p_pCount = ( *Itr ).ParamCount;
				return;
			}
			if( ( *Itr ).CommandName.compare( p_Command ) == 0 )
			{
				*p_pCount = ( *Itr ).ParamCount;
				return;
			}
			if( ( *Itr ).ShortCmdName.compare( p_Command ) == 0 )
			{
				*p_pCount = ( *Itr ).ParamCount;
				return;
			}
		}

		Updater::Command::GetCommandParamCount( p_Command, p_pCount );
	}

	void ProgramCommands::GetCommandList( std::list< std::string > &p_Commands )
	{
	}

	int DisplayUsage( const char *pNULL, ProgramCommands *p_pSelf )
	{
		std::cout << "USAGE" << std::endl;
		return 0;
	}

	int SetConfigurationFile( const char *p_pFileName,
		ProgramCommands *p_pSelf )
	{
		if( p_pFileName == NULL || strlen( p_pFileName ) == 0 )
		{
			return 1;
		}

		p_pSelf->m_ConfigurationFile.clear( );
		p_pSelf->m_ConfigurationFile.append( p_pFileName );

		return 0;
	}

	int SetSite( const char *p_pSite, ProgramCommands *p_pSelf )
	{
		if( p_pSite == NULL || strlen( p_pSite ) == 0 )
		{
			return 1;
		}

		p_pSelf->m_Site.clear( );
		p_pSelf->m_Site.append( p_pSite );

		return 0;
	}

	int SetPort( const char *p_pPort, ProgramCommands *p_pSelf )
	{
		if( p_pPort == NULL || strlen( p_pPort ) == 0 )
		{
			return 1;
		}
		
		p_pSelf->m_Port.clear( );
		p_pSelf->m_Port.append( p_pPort );

		return 0;
	}

	int SetActiveMode( const char *p_pMode, ProgramCommands *p_pSelf )
	{
		if( p_pMode == NULL || strlen( p_pMode ) == 0 )
		{
			return 1;
		}

		if( strcmp( p_pMode, "true" ) == 0 )
		{
			p_pSelf->m_ActiveMode = true;
		}
		else if( strcmp( p_pMode, "false" ) == 0 )
		{
			p_pSelf->m_ActiveMode = false;
		}

		return 0;
	}

	int SetPortRange( const char *p_pPortRange, ProgramCommands *p_pSelf )
	{
		// Get the delimited values, then try to convert them to
		// their integer values
		std::string Port1, Port2, Range;
		Range.clear( );
		Range.append( p_pPortRange );
		size_t Delimiter = 0;
		Delimiter = Range.find_first_of( ":" );
		if( Delimiter != std::string::npos )
		{
			Port1 = Range.substr( 0, Delimiter );
			Port2 = Range.substr( Delimiter+1, Range.size( ) );
			p_pSelf->m_PortRange.Port[ 0 ] = atoi( Port1.c_str( ) );
			p_pSelf->m_PortRange.Port[ 1 ] = atoi( Port2.c_str( ) );
		}
		return 0;
	}

	int SetProject( const char *p_pProject, ProgramCommands *p_pSelf )
	{
		p_pSelf->m_Project.clear( );
		p_pSelf->m_Project.append( p_pProject );

		return 0;
	}

	int SetPlatform( const char *p_pPlatform, ProgramCommands *p_pSelf )
	{
		if( p_pPlatform == NULL || strlen( p_pPlatform ) == 0 )
		{
			return 1;
		}

		p_pSelf->m_Platform.clear( );
		p_pSelf->m_Platform.append( p_pPlatform );

		return 0;
	}

	int SetBuildType( const char *p_pBuildType, ProgramCommands *p_pSelf )
	{
		if( p_pBuildType == NULL || strlen( p_pBuildType ) == 0 )
		{
			return 1;
		}

		p_pSelf->m_BuildType.clear( );
		p_pSelf->m_BuildType.append( p_pBuildType );

		return 0;
	}

	int SetDownloadSummary( const char *p_pDownloadSummary,
		ProgramCommands *p_pSelf )
	{
		if( p_pDownloadSummary == NULL || strlen( p_pDownloadSummary ) == 0 )
		{
			return 1;
		}

		p_pSelf->m_DownloadSummary.clear( );
		p_pSelf->m_DownloadSummary.append( p_pDownloadSummary );

		return 0;
	}
}
