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

int GetConfigFile( const char *p_pParameter, char **p_pConfigFile );
int GetSite( const char *p_pParameter, char **p_pSite );
int GetPort( const char *p_pParameter, char **p_pPort );
int GetProject( const char *p_pParameter, char **p_pProject );
int GetPlatform( const char *p_pParameter, char **p_pPlatform );
int GetBuildType( const char *p_pParameter, char **p_pBuildType );
int GetDownloadSummary( const char *p_pParameter, char **p_pDownloadSummary );
int GetActiveMode( const char *p_pParameter, bool &p_ActiveMode );
int GetPortRange( const char *p_pParameter, int **p_pRange );

void DisplayUsage( );

typedef int ( *COMMAND_FUNCTION )( void *p_pParameters );

typedef enum __PARAM_TYPE
{
	PARAM_INTEGER	= 1,
	PARAM_FLOAT,
	PARAM_STRING,
	PARAM_UNKNOWN,
}PARAM_TYPE;

typedef struct __COMMAND
{
	// name function num params param list
	std::string			Name;
	COMMAND_FUNCTION	Function;
	size_t				ParamCount;
	PARAM_TYPE			*pParams;
}COMMAND;

class Commands
{
public:
	Commands( );
	~Commands( );

	int Add( const std::string &p_Name, const COMMAND_FUNCTION p_Function,
		const size_t p_ParamCount, const PARAM_TYPE *p_pParamList );

	void GetCommandList( std::list< std::string > &p_Commands );

	int Execute( const char *p_pCommandName,
		const void *p_pParameters );

private:
	std::list< COMMAND > m_Commands;
};

#endif
