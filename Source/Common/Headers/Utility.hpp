#ifndef __UPDATER_UTILITY_HPP__
#define __UPDATER_UTILITY_HPP__

#define SAFE_DELETE_ARRAY( Array )\
	{\
		if( Array != NULL )\
		{\
			delete [ ] Array;\
			Array = NULL;\
		}\
	}

#ifdef PLATFORM_WINDOWS
int ConvertCharToWide( const char *p_pChar, wchar_t **p_ppWide );
#endif

#endif
