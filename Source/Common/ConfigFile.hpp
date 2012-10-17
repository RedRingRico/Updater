#ifndef __UPDATER_CONFIGFILE_HPP__
#define __UPDATER_CONFIGFILE_HPP__

#include <Windows.h>
#inlcud <string>

class ConfigFile
{
public:
	ConfigFile( );
	~ConfigFile( );

private:

#ifdef PLATFORM_LINUX
#elif PLATFORM_WINDOWS
	HANDLE m_FileHandle;
	std::wstring m_FileName;
#endif

};

#endif
