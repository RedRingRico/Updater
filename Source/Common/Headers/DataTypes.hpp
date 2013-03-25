#ifndef __UPDATER_DATATYPES_HPP__
#define __UPDATER_DATATYPES_HPP__

#ifdef PLATFORM_WINDOWS
#include <DataTypes_Windows.hpp>
#elif defined PLATFORM_LINUX
#include <DataTypes_Linux.hpp>
#else
#error Unknown platform
#endif

#endif

