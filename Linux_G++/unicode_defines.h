

#ifndef _UNICODE_DEFINES_INCLUDED_
#define _UNICODE_DEFINES_INCLUDED_

/**
 * unfortunately we need to include this because otherwise the typedef will fail.
 * in MSVC we can use _STRING_, but that is non-standard.
 */ 
#include <string>
#include <fstream>

#include <cstdio>

#ifdef _UNICODE

#define strtol_t wcstol
#define strtod_t wcstod
#define strcspn_t wcscspn
#define vsprintf_t  vswprintf

#ifdef _WIN32
#define vsnprintf_t _vsnwprintf
#else
#define vsnprintf_t vsnwprintf
#endif

typedef std::wifstream TSTREAM;
typedef std::wstring TSTRING;

#else

#define strtol_t strtol
#define strtod_t strtod
#define strcspn_t strcspn
#define vsprintf_t  vsprintf

#ifdef _WIN32
#define vsnprintf_t _vsnprintf
#else
#define vsnprintf_t vsnprintf
#endif

typedef std::ifstream TSTREAM;
typedef std::string TSTRING;

#endif /* _UNICODE */

#endif
