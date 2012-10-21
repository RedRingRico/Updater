#ifndef __UPDATER_COMMANDS_HPP__
#define __UPDATER_COMMANDS_HPP__

#include <list>

// Integer values of command-line arguments
const int INVALID_COMMAND			= 0xFFFFFFFF;

enum
{
	COMMAND_CONFIG_FILE = 0,
	COMMAND_SITE,
	COMMAND_PORT,
	COMMAND_PROJECT,
	COMMAND_PLATFORM,
	COMMAND_BUILD_TYPE,
	COMMAND_DOWNLOAD_SUMMARY,
	COMMAND_ACTIVE_MODE,
	COMMAND_PORT_RANGE,
	COMMAND_USAGE,
	MAX_COMMANDS
};

// Returns the integer value from the command line
int GetCommandLineParameter( const char *p_pArg, char **p_ppParameter );

typedef int ( *COMMAND_FUNCTION )( const char **p_pParameters );

typedef enum __PARAM_TYPE
{
	PARAM_INTEGER	= 1,
	PARAM_BOOLEAN,
	PARAM_FLOAT,
	PARAM_STRING,
	PARAM_UNKNOWN,
}PARAM_TYPE;

typedef struct __COMMAND
{
	// The function name to call the function by
	std::string			FunctionName;
	// The command-line equivalents for calling the function
	// (both the long and one-character versions)
	std::string			CommandName;
	std::string			ShortCmdName;
	// The actual function to call
	COMMAND_FUNCTION	Function;
	// How many parameters this function takes and the type
	size_t				ParamCount;
	PARAM_TYPE			*pParams;
}COMMAND;

namespace Updater
{
	class Command
	{
	public:
		Command( );
		~Command( );

		virtual int Add( const std::string &p_FunctionName,
			const std::string &p_CommandName, const std::string &p_ShortCmdName,
			const COMMAND_FUNCTION p_Function,
			const size_t p_ParamCount, const PARAM_TYPE *p_pParams );

		// Use this to determine how many ppargv[ n ]'s to read
		virtual void GetCommandParamCount( std::string p_Command,
			int *p_pCount );
		virtual void GetCommandList( std::list< std::string > &p_Commands );

		virtual int Execute( const char *p_pCommandName,
			const char **p_pParameters );

	protected:
		typedef std::list< COMMAND >::iterator CommandItr;
		std::list< COMMAND > m_Commands;
	};
}

#endif
