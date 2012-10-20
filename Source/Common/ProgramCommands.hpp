#ifndef __UPDATER_PROGRAMCOMMANDS_HPP__
#define __UPDATER_PROGRAMCOMMANDS_HPP__

#include <string>

// Global variables for holding the program's configuration state
typedef struct __PORTRANGE
{
	int Port[ 2 ];
}PORTRANGE;

static PORTRANGE g_PortRange;

static std::string g_Site;
static std::string g_Port;
static std::string g_Project;
static std::string g_Platform;
static std::string g_BuildType;
static std::string g_DownloadSummary;
static bool g_ActiveMode;

int DisplayUsage( void *pNULL );

int SetSite( void *pSite );

int SetPortRange( void *pPortRange );

#endif
