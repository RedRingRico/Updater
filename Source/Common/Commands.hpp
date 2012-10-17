#ifndef __UPDATER_COMMANDS_HPP__
#define __UPDATER_COMMANDS_HPP__

// Integer values of command-line arguments
const int COMMAND_CONFIG_FILE		= 0x00000001;
const int COMMAND_SITE				= 0x00000002;
const int COMMAND_PORT				= 0x00000004;
const int COMMAND_PROJECT			= 0x00000008;
const int COMMAND_PLATFORM			= 0x00000010;
const int COMMAND_BUILD_TYPE		= 0x00000020;
const int COMMAND_DOWNLOAD_SUMMARY	= 0x00000040;
const int COMMAND_ACTIVE_MODE		= 0x00000080;
const int COMMAND_PORT_RANGE		= 0x00000100;
const int INVALID_COMMAND			= 0xFFFFFFFF;

// Returns the integer value from the command line
int GetCommandLineParameter( const char *p_pArg, char *p_pParameter );

int GetSite( const char *p_pParameter, char *p_pSite );
int GetPort( const char *p_pParameter, char *p_pPort );
int GetProject( const char *p_pParameter, char *p_pProject );
int GetPlatform( const char *p_pParameter, char *p_pPlatform );
int GetBuildType( const char *p_pParameter, char *p_pBuildType );
int GetDownloadSummary( const char *p_pParameter, char *p_pDownloadSummary );
int GetActiveMode( const char *p_pParameter, bool &p_ActiveMode );
int GetPortRange( const char *p_pParameter, int *p_pRange );

#endif
