
#ifndef _UTILITY_H_
#define _UTILITY_H_

/**
 * @author Ricky Neil
 * @file utility.h
 * File containing utility functions which are needed by the program to operate.
 * These funcitons do not belong to any one class so they are extracted and put here.
 */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <string>

#include "unicode_defines.h"

namespace util
{

/**
 * Lower Limit Function.
 * @param base value to apply lower limit to.
 * @param cap value to use as a lower limit.
 * @return base if base is above cap, else returns cap.
 */
template<typename T> T
ALIML( T base, T cap )
{
	return ( base < cap ) ? cap : base;
}

/**
 * upper Limit Function.
 * @param base value to apply upper limit to.
 * @param cap value to use as a upper limit.
 * @return base if base is below cap, else returns cap.
 */
template<typename T> T
ALIMU( T base, T cap )
{
	return ( base > cap ) ? cap : base;
}

/**
 * upper and lower Limit Function.
 * @param base value to apply upper and lower limit to.
 * @param cap_l value to use as a lower limit.
 * @param cap_l value to use as a upper limit.
 * @return base if base is above cap, else returns cap.
 */
template<typename T> T
ALIMA( T base, T cap_u, T cap_l )
{
	return ALIML( ALIMU( base, cap_u ), cap_l );
}

/**
 * Trims leading non-graphical charactors from a string in-place.
 * http://stackoverflow.com/questions/216823/.
 * @param s string to trim non-graphical from.
 * @return same string with non-graphical trimmed.
 */
TSTRING& left_trim( TSTRING& s );

/**
 * Trims trailing non-graphical charactors from a string in-place.
 * http://stackoverflow.com/questions/216823/.
 * @param s string to trim non-graphical from.
 * @return same string with non-graphical trimmed.
 */
TSTRING& right_trim( TSTRING& s );

/**
 * Trims leading and trailing non-graphical charactors from a string in-place.
 * http://stackoverflow.com/questions/216823/.
 * @param s string to trim non-graphical from.
 * @return same string with non-graphical trimmed.
 */
TSTRING& trim( TSTRING& s );

/**
 * Parses an Int64 into its base 10 string representation.
 * Can be used for Int8, Int16, Int32, supports negative numbers.
 * @param num number to parse.
 * @return string containing the numbers string representation.
 */
TSTRING Int64ToString( const INT64 num );

/**
 * Parses a string into an Int16
 * @param str string to parse.
 * @param base number system to parse to.
 * @return 0 on failure, parsed number on success.
 */
INT16 StringToInt16( const TSTRING& str, const int base = 0 );

/**
 * Parses a string into an Int32
 * @param str string to parse.
 * @param base number system to parse to.
 * @return 0 on failure, parsed number on success.
 */
INT32 StringToInt32( const TSTRING& str, const int base = 0 );

/**
 * Parses a string into an Int64
 * @param str string to parse.
 * @param base number system to parse to.
 * @return 0 on failure, parsed number on success.
 */
INT64 StringToInt64( const TSTRING& str, const int base = 0 );

/**
 * Parses a string into an Double
 * @param str string to parse.
 * @return 0 on failure, parsed number on success.
 */
double StringToDouble( const TSTRING& str );

/**
 * Converts an array of charactors into their hex representation.
 * @param str array of chars to convert into hex.
 * @param size size of the array str.
 * @return string containing the hex representation of the str array.
 */
TSTRING toHex( const TCHAR* const str, const int size );

/**
 * Converts a hex string to a binary TCHAR string.
 * @note Input string str will be converted to upper case hex.
 * @param str hex string to be converted to binary chars.
 * @return string containing the binary representation of the hex string.
 */
TSTRING fromHex( TSTRING str );

}

#endif
