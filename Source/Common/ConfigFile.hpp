#ifndef __UPDATER_CONFIGFILE_HPP__
#define __UPDATER_CONFIGFILE_HPP__

#include <Windows.h>
#include <string>
#include <map>

class ConfigFile
{
public:
	ConfigFile( );
	ConfigFile( const char *p_pConfigFile );

	~ConfigFile( );

	int SetConfigFile( const char *p_pConfigFile );

	int Parse( );

	int GetValue( const std::string &p_Option, std::string &p_Value );

	void GetOptionValue( const size_t p_Index, std::string &p_Option,
		std::string &p_Value );

private:
	void RemoveWhitespace( std::string &p_String, const bool p_Front = true,
		const bool p_Back = true );

#ifdef PLATFORM_LINUX
	FILE *m_pFile;
#elif PLATFORM_WINDOWS
	HANDLE m_FileHandle;
	std::wstring m_FileName;
#endif
	
	// Save some typing!
	typedef std::map< std::string, std::string > OptionValueMap;
	typedef std::map< std::string, std::string >::iterator OptionValueItr;

	OptionValueMap m_OptionValue;
};

#endif
