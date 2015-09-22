
#ifndef _CONFIG_LOADER_H_
#define _CONFIG_LOADER_H_

/**
 * @author Ricky Neil
 * @file config_loader.h
 * File containing ConfigLoader and a DefaultParser.
 */

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <queue>
#include <string>
#include <unordered_map>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#include "unicode_defines.h"
#include "config_types.h"

/**
 * Acts as a default configuration file parser.
 * Includes String, Short, Int, Long and Double config entries.
 */
class DefaultParser : public Parser<TSTRING>
{
public:
	/**
	 * Constructor, does nothing except call base constructor.
	 * @param sectionName name of section in config file to wrap this parser around.
	 */
	DefaultParser( const TSTRING& sectionName )
		: Parser( sectionName ) {};

	/**
	 * Add a key, value pair to this parsers dictionary.
	 * @param key config file key to use for lookup.
	 * @param value to store in lookup keys bucket.
	 */
	void Parse( const TSTRING& key, const TSTRING& value )
	{
		if ( Configuration.count( key ) == 0 )
		{
			Configuration[key] = value;
		}
		else
		{
			message = TEXT("Duplicate Configuration Key: ") + key;
		}
	}

	/**
	 * Gets a string from the dictionary.
	 * @param key key to use when looking for a value in the dictionary.
	 * @param Default value to return when the key lookup fails.
	 * @return string which is either value returned from lookup or Default.
	 */
	TSTRING getString( const TSTRING& key, const TSTRING& Default )
	{
		return GetItem( key, Default );
	}

	/**
	 * Gets an Int16 from the dictionary.
	 * @param key key to use when looking for a value in the dictionary.
	 * @param Default value to return when the key lookup fails.
	 * @return Int16 which is either value returned from lookup or Default.
	 */
	INT16 getInt16( const TSTRING& key, const INT16 Default )
	{
		TSTRING item = GetItem( key, TEXT("") );
		return ( item != TEXT("") ) ? util::StringToInt16( item ) : Default;
	}
	
	/**
	 * Gets an Int32 from the dictionary.
	 * @param key key to use when looking for a value in the dictionary.
	 * @param Default value to return when the key lookup fails.
	 * @return Int32 which is either value returned from lookup or Default.
	 */
	INT32 getInt32( const TSTRING& key, const INT32 Default )
	{
		TSTRING item = GetItem( key, TEXT("") );
		return ( item != TEXT("") ) ? util::StringToInt32( item ) : Default;
	}
	
	/**
	 * Gets an Int64 from the dictionary.
	 * @param key key to use when looking for a value in the dictionary.
	 * @param Default value to return when the key lookup fails.
	 * @return Int64 which is either value returned from lookup or Default.
	 */
	INT64 getInt64( const TSTRING& key, const INT64 Default )
	{
		TSTRING item = GetItem( key, TEXT("") );
		return ( item != TEXT("") ) ? util::StringToInt64( item ) : Default;
	}

	/**
	 * Gets an Double from the dictionary.
	 * @param key key to use when looking for a value in the dictionary.
	 * @param Default value to return when the key lookup fails.
	 * @return Double which is either value returned from lookup or Default.
	 */
	double getDouble( const TSTRING& key, const double Default )
	{
		TSTRING item = GetItem( key, TEXT("") );
		return ( item != TEXT("") ) ? util::StringToDouble( item ) : Default;
	}
};

class ConfigHandle; /**< Forward delceration just for the header file */
typedef std::auto_ptr<ConfigHandle> CONFIGHANDLE;

/**
 * Global Configuration File Handler.
 * Designed to allow flexable allocation of .ini structured configuration files.\n
 * This class was designed to provide extendable configuration file reader which could be used
 * at any location within a program.\n
 * Adding this class will allow access to the global dictionary of configuration files and configuration Parsers.\n
 * With the help of configurable parsers this class allows all types of configuration entries to be read and parsed in any way.\n
 */
class ConfigLoader
{

protected:

	/**
	 * @param key Name of Configuration File.
	 * @param value Vector of strings corresponding to lines in the Configuration file.
	 */
	typedef std::unordered_map<TSTRING, std::vector<TSTRING>> FileMapping;

	/**
	 * @param key name of the section to hook a parser to.
	 * @param value parser to hook into the config file section.
	 */
	typedef std::unordered_map<TSTRING, ParserBase*> StorageMap;

	/**
	 * @param key name of the configuration file.
	 * @param value config loader class hooked into the configuration file.
	 */
	typedef std::unordered_map<TSTRING, ConfigLoader*> ConfigMap;

	int references; /**< number of instances of this class that are currently in use */

	FileMapping FileMap; /**< Map of the file that this ConfigLoader is hooked into. */
	StorageMap Sections; /**< Sections in the config file that have a parser hooked into them */

	static ConfigMap OpenConfigs; /**< Stores instances for all open config files, avaliable to all config loaders */

	TSTRING file_name; /**< name of the file to use for this ConfigLoader */

	int max_messages; /**< Maximum number of messages allowed in the message queue. */
	std::queue<TSTRING> message_queue; /**< queue of messages used for errors and reports. */

	/**
	 * Constructor
	 * @param filename name of the config file to hook into with this config loader.
	 */
	ConfigLoader( const TSTRING& filename );

	/**
	 * Removes file extentions from the file name.
	 * @param filename name of the file to be sanitised.
	 * @return filename with extnsion stripped
	 */
	static TSTRING RemoveExtension( const TSTRING& filename );

	/**
	 * Loads the file into the ConfigLoader class and reads it into memory.
	 */
	void LoadFile();

	/**
	 * Adds a message to the message queue.
	 * If the max queue size has been reached will increment "other messages" counter.
	 * @param message template string to be added to queue.
	 * @param ... arguments to be used in the message template.
	 */
	void AddMessage( TSTRING message, ... );

	/**
	 * Virtual Destructor.
	 * Cleans up this class
	 */
	virtual ~ConfigLoader();

public:

	/**
	 * Function to either create a new ConfigLoader or return an existing one.
	 * @param filename name of the config file to hook into.
	 * @return pointer to either a new or existing ConfigLoader.
	 */
	static CONFIGHANDLE InitialiseConfig( const TSTRING& filename );

	/**
	 * Adds a section to the ConfigLoader.
	 * @param section pointer to the Parser to use.
	 * @return success or failure.
	 */
	bool AddSection(ParserBase* section);

	/**
	 * Returns the parser hooked into a section, should be cast from base to actual.
	 * @param section_name name of the section to get parser for.
	 * @return base class of the parser hooked into the section.
	 */
	ParserBase* GetSection(const TSTRING& section_name);

	/**
	 * Unhooks a Parser from a Section.
	 * @param section_name name of the section to Unhook from.
	 */
	void DeleteSection(const TSTRING& section_name);

	/**
	 * returns the messages stored in the message_queue.
	 * returns an empty string if no messages.
	 * @return the top message from the message queue.
	 */
	TSTRING PollMessages();

	/**
	 * Returns the parser hooked into a section in a file, should be cast from base to actual.
	 * @param file filename of file to get section from.
	 * @param section name of the section to get parser for. 
	 * @return base class of the parser hooked into the section.
	 */
	static ParserBase* GetFileSection(const TSTRING& file, const TSTRING& section);

	/**
	 * Close all configuration files.
	 * @param force close even if other references are still open.
	 */
	static void CloseAll( const bool force = false );

	/**
	 * Close Config with Config Pointer.
	 * @param config pointer to the ConfigLoader to close.
	 * @param force close even if other references are still open.
	 */
	static void CloseConfig(ConfigLoader* config, const bool force = false);

	/**
	 * Close Config with file name.
	 * @param ini_file name of file with a ConfigLoader Hooked to it.
	 * @param force close even if other references are still open.
	 */
	static void CloseConfig(const TSTRING& ini_file, const bool force = false);
};

class ConfigHandle
{
	ConfigLoader* config;

public:
	ConfigHandle( ConfigLoader* con )
		: config( con ) {}


	/**
	 * Adds a section to the ConfigLoader.
	 * @param section pointer to the Parser to use.
	 * @return success or failure.
	 */
	bool AddSection( ParserBase* section )
	{
		config->AddSection( section );
	}

	/**
	 * Returns the parser hooked into a section, should be cast from base to actual.
	 * @param section_name name of the section to get parser for.
	 * @return base class of the parser hooked into the section.
	 */
	ParserBase* GetSection(const TSTRING& section_name)
	{
		config->GetSection( section_name );
	}

	/**
	 * Unhooks a Parser from a Section.
	 * @param section_name name of the section to Unhook from.
	 */
	void DeleteSection(const TSTRING& section_name)
	{
		config->DeleteSection( section_name );
	}

	/**
	 * returns the messages stored in the message_queue.
	 * returns an empty string if no messages.
	 * @return the top message from the message queue.
	 */
	TSTRING PollMessages()
	{
		return config->PollMessages();
	}

	~ConfigHandle()
	{
		ConfigLoader::CloseConfig( config );
	}
};

#define OPEN_CONFIG ConfigLoader::InitialiseConfig

#endif
