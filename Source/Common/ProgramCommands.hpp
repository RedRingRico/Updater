#ifndef __UPDATER_PROGRAMCOMMANDS_HPP__
#define __UPDATER_PROGRAMCOMMANDS_HPP__

#include <Commands.hpp>
#include <string>

typedef struct __PORTRANGE
{
	int Port[ 2 ];
}PORTRANGE;

namespace Updater
{
	class ProgramCommands : public Command
	{
		typedef int ( *CLASS_COMMAND_FUNCTION )( const char **p_pParameters,
			ProgramCommands *p_pSelf );
			typedef struct __CLASS_COMMAND
		{
			// The function name to call the function by
			std::string			FunctionName;
			// The command-line equivalents for calling the function
			// (both the long and one-character versions)
			std::string			CommandName;
			std::string			ShortCmdName;
			// The actual function to call
			CLASS_COMMAND_FUNCTION	Function;
			// How many parameters this function takes and the type
			size_t				ParamCount;
			PARAM_TYPE			*pParams;
		}CLASS_COMMAND;

	public:
		ProgramCommands( );
		~ProgramCommands( );

		int Initialise( );
		
		virtual void GetCommandParamCount( std::string p_Command,
			int *p_pCount );
		virtual void GetCommandList( std::list< std::string > &p_Commands );

		virtual int ValidCommand( const std::string &p_Command );

		virtual int Execute( const char *p_pCommandName,
			const char **p_pParameters );

		// Accessors for the configuration data
		const char *GetConfigurationFile( )
			{ return m_ConfigurationFile.c_str( ); }
		const char *GetSite( ){ return m_Site.c_str( ); }
		const char *GetPort( ){ return m_Port.c_str( ); }
		const char *GetProject( ){ return m_Project.c_str( ); }
		const char *GetPlatform( ){ return m_Platform.c_str( ); }
		const char *GetBuildType( ){ return m_BuildType.c_str( ); }
		const char *GetDownloadSummary( ){ return m_DownloadSummary.c_str( ); }

		const void GetPortRange( int **p_pPorts )
			{	( *p_pPorts )[ 0 ] = m_PortRange.Port[ 0 ];
				( *p_pPorts )[ 1 ] = m_PortRange.Port[ 1 ]; }
		const bool GetActiveMode( ){ return m_ActiveMode; }

	private:
		virtual int Add( const std::string &p_FunctionName,
			const std::string &p_CommandName, const std::string &p_ShortCmdName,
			const CLASS_COMMAND_FUNCTION p_Function,
			const size_t p_ParamCount, const PARAM_TYPE *p_pParams,
			ProgramCommands *p_pSelf );

		friend int DisplayUsage( const char **p_pNULL,
			ProgramCommands *p_pSelf );
		friend int SetConfigurationFile( const char **p_pFileName,
			ProgramCommands *p_pSelf );
		friend int SetSite( const char **p_pSite, ProgramCommands *p_pSelf );
		friend int SetPort( const char **p_pPort, ProgramCommands *p_pSelf );
		friend int SetActiveMode( const char **p_ppMode,
			ProgramCommands *p_pSelf );
		friend int SetPortRange( const char **p_pPortRange,
			ProgramCommands *p_pSelf );
		friend int SetProject( const char **p_pProject,
			ProgramCommands *p_pSelf );
		friend int SetPlatform( const char **p_pPlatform,
			ProgramCommands *p_pSelf );
		friend int SetBuildType( const char **p_pType,
			ProgramCommands *p_pSelf );
		friend int SetDownloadSummary( const char **p_pDownloadSummary,
			ProgramCommands *p_pSelf );	

		PORTRANGE m_PortRange;

		std::string m_ConfigurationFile;
		std::string m_Site;
		std::string m_Port;
		std::string m_Project;
		std::string m_Platform;
		std::string m_BuildType;
		std::string m_DownloadSummary;

		bool m_ActiveMode;

		typedef std::list< CLASS_COMMAND >::iterator ClassCmdItr;
		std::list< CLASS_COMMAND > m_ClassCommands;
	};
}

#endif
