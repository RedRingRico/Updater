#ifndef __UPDATER_CONFIGFILE_HPP__
#define __UPDATER_CONFIGFILE_HPP__

#include <Windows.h>
#include <string>

typedef struct __CONFIG_PARAMETERS
{
	char *pHost;
	char *pProject;
	char *pBuildType;
	char Port;
	bool ActiveMode;
	int PortRange[ 2 ];
	bool OutputDownloadSummary;
}CONFIG_PARAMETERS;

class ConfigFile
{
public:
	ConfigFile( );
	ConfigFile( const wchar_t *p_pConfigFile );

	~ConfigFile( );

	int SetConfigFile( const wchar_t *p_pConfigFile );

	int Parse( CONFIG_PARAMETERS *p_pParameters );

private:

#ifdef PLATFORM_LINUX
#elif PLATFORM_WINDOWS
	HANDLE m_FileHandle;
	std::wstring m_FileName;
#endif

};

#endif
