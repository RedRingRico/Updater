#ifndef __UPDATER_COMMANDS_HPP__
#define __UPDATER_COMMANDS_HPP__

// Integer values of command-line arguments
const int COMMAND_CONFIG_FILE		= 0;
const int COMMAND_SITE				= 1;
const int COMMAND_PORT				= 2;
const int COMMAND_PROJECT			= 3;
const int COMMAND_PLATFORM			= 4;
const int COMMAND_BUILD_TYPE		= 5;
const int COMMAND_DOWNLOAD_SUMMARY	= 6;
const int COMMAND_ACTIVE_MODE		= 7;
const int COMMAND_PORT_RANGE		= 8;
const int COMMAND_USAGE				= 9;
const int INVALID_COMMAND			= 0xFFFFFFFF;

const int MAX_COMMANDS	= 10;


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

#endif
