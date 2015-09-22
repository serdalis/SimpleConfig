
#include "utility.h"

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace util
{

TSTRING&
left_trim( TSTRING& s )
{
	s.erase( s.begin(), std::find_if( s.begin(), s.end(), std::ptr_fun<int, int>( std::isgraph ) ) );
	return s;
}


TSTRING&
right_trim( TSTRING& s )
{
	s.erase( std::find_if( s.rbegin(), s.rend(), std::ptr_fun<int, int>( std::isgraph ) ).base(), s.end() );
	return s;
}


TSTRING&
trim( TSTRING& s )
{
	return left_trim( right_trim( s ) );
}


TSTRING
Int64ToString( const INT64 num )
{
	int i = 0;
	int neg = 0;
	int digits = 0;

	INT64 p_num = num;

	TSTRING str( TEXT("0") );

	if ( num < 0 )
	{
		neg = 1;
		p_num = -num;
	}

	for ( i = 1; i <= p_num; i *= 10 )
	{
		digits += 1;
	}

	if ( digits > 0 )
	{
		i = digits + neg;
		str.resize( i );

		if ( neg ) str[0] = '0';

		for ( i = i - 1; p_num > 0; i -= 1 )
		{
			str[i] = '0' + ( p_num % 10 );
			p_num = (INT64) ( p_num * 0.1 );
		}
	}

	return str;
}



INT16
StringToInt16( const TSTRING& str, const int base )
{
	return static_cast<INT16>( StringToInt64(str, base) );
}


INT32
StringToInt32( const TSTRING& str, const int base )
{
	return static_cast<INT32>( StringToInt64(str, base) );
}


INT64
StringToInt64( const TSTRING& str, const int base )
{
	TCHAR* end;
	return strtol_t( str.c_str(), &end, base );
}

static const TCHAR hex[] = { TEXT("0123456789ABCDEF") };

TSTRING
toHex( const char* const str, const int size )
{
	TSTRING hex_string('0', size*2 + 1);

	int i = 0;
	for(i = 0; i < size; ++i)
	{
		hex_string[(i*2)] = hex[(str[i] >> 8) & 0x0F];
		hex_string[(i*2)+1] = hex[str[i] & 0x0F];
	}

	hex_string[size*2] = '\0';
	return hex_string;
}

TSTRING
fromHex( TSTRING str )
{
	TSTRING::size_type i = 0;
	TSTRING bin;
	bin.reserve(str.size() / 2);
	std::transform( str.begin(), str.end(), str.begin(), ::toupper );
	
	for ( i = 0; i < str.size(); i += 2 )
	{
		bin += (TCHAR)( (strcspn_t( hex, &str[i] )*0x10) + strcspn_t( hex, &str[i+1] ) );
	}
	return bin;
}


double
StringToDouble( const TSTRING& str )
{
	TCHAR* end;
	return strtod_t( str.c_str(), &end );
}

}