#include <ProgramCommands.hpp>
#include <iostream>

int DisplayUsage( void *pNULL )
{
	std::cout << "USAGE" << std::endl;
	return 0;
}

int SetSite( void *p_pSite )
{
	char *pSite = reinterpret_cast< char * >( p_pSite );

	std::cout << "PARAMSITE: " << pSite << std::endl;

	return 0;
}

int SetPortRange( void *p_pPortRange )
{
	g_PortRange = *( ( PORTRANGE* )p_pPortRange );

	std::cout << "Range " << g_PortRange.Port[ 0 ] << ":" << g_PortRange.Port[ 1 ] <<
		std::endl;

	
	return 0;
} 