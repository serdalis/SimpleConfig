
#ifndef _CONFIG_TYPES_H_
#define _CONFIG_TYPES_H_

/**
 * @author Ricky Neil
 * @file config_types.h
 * File containing configuration file parser bases.
 * Can be extended to include any additional parsers that the config file will use.
 */

#include <map>
#include <string>
#include "utility.h"

/**
 * Base Class for ConfigLoader Parsers.
 * Designed to allow parsers to be stored in collections.
 * All parser types should inherit from the templated version of this class.
 *
 * @warning This class is not to be used directly.
 */
class ParserBase
{
protected:

	TSTRING message; /**< Message will be set when an unexpected event happens. */

	/**
	 * Constructor
	 * @param sectionName name of the section being hooked into.
	 */
	ParserBase(const TSTRING& sectionName)
		: section_name(sectionName), auto_key(0) {};

public:
	int auto_key;		  /**< last used auto generated key value. */
	TSTRING section_name; /**< Name of section this is hooked into. */

	/**
	 * Virtual function which will add to the parsers dictionary.
	 * @param key key to be used for lookups.
	 * @param value value to be stored in the dictionary.
	 */
	virtual void Parse( const TSTRING& key, const TSTRING& value ) {};

	TSTRING CheckMessage();
};

/**
 * Templated Base type for Parsers, allows different types of parsers.
 * Only minimum functionality is avaliable in this parser.\n
 *
 * @note Inherit from this class to create custom parsers.
 *
 * @tparam ItemType type of the item to be stored in this configuration section.
 */
template <class ItemType>
class Parser : public ParserBase
{
protected:
	/**
	 * Internal map type definition for use with iterators.
	 * @param key name of the configuration entry.
	 * @param value storage for the configuration item.
	 */
	typedef std::map<TSTRING, ItemType> MapType;

	MapType Configuration; /**< Parser Dictionary */

	/**
	 * Constructor
	 * @param sectionName name of the section being hooked into.
	 */
	Parser( TSTRING sectionName )
		: ParserBase( sectionName ) {};

public:

	/**
	 * Virtual function which will add to the parsers dictionary.
	 * @param key key to be used for lookups.
	 * @param value value to be stored in the dictionary.
	 */
	virtual void Parse( const TSTRING& key, const ItemType& value )
	{
		if ( Configuration.count( key ) == 0 )
		{
			Configuration[key] = value;
		}
		else
		{
			message = TEXT("Duplicate Configuration Key: %s\n") + key;
		}
	}

	/**
	 * Inner Get function for the Parsers.
	 * @param key key to be used for lookups.
	 * @param Default value to return when the key lookup fails.
	 * @return Item which is either value returned from lookup or Default.
	 */
	const ItemType GetItem( const TSTRING& key, const ItemType Default )
	{
		if ( Configuration.count( key ) != 0 )
		{
			return Configuration[key];
		}
		return Default;
	}

	/**
	 * Gets the Key of the item at the index specified.
	 * @param index index to look at for the key.
	 * @return string containing the key of the item at index.
	 */
	TSTRING GetAt( const int index )
	{
		int i = index;
		MapType::iterator mit = Configuration.begin();
		/* maps don't actually have indexes, this is a hack just
		 * so we can access every element without knowing its key */
		while ( i && mit != Configuration.end() )
		{
			++mit;
			--i;
		}

		return ( mit == Configuration.end() ) ? "" : mit->first;
	}
};

#endif
