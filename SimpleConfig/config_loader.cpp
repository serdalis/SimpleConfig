#include "config_loader.h"

#include <vector>
#include <Shlwapi.h>
#include <algorithm>

#pragma comment( lib, "Shlwapi.lib" )

ConfigLoader::ConfigMap ConfigLoader::OpenConfigs;

CONFIGHANDLE
ConfigLoader::InitialiseConfig( const TSTRING& filename, const TSTRING& path )
{
	TSTRING sanitised = RemoveExtension( filename );

	if( OpenConfigs.count( sanitised ) )
	{
		OpenConfigs[sanitised]->references += 1;
	}
	else
	{
		OpenConfigs[sanitised] = new ConfigLoader( filename, path );
	}

	return CONFIGHANDLE( new ConfigHandle( OpenConfigs[sanitised] ) );
}


TSTRING
ConfigLoader::RemoveExtension( const TSTRING& filename )
{
	TSTRING::size_type ext = filename.rfind( '.' );
	TSTRING newFilename = filename.substr( 0, ext );

	return newFilename;
}


ConfigLoader::ConfigLoader( const TSTRING& filename, const TSTRING& path )
{
	fileName = filename;
	filePath = path;

	references = 1;

	max_messages = 100;

	LoadFile();
}


void
ConfigLoader::AddMessage( TSTRING message, ... )
{
	std::vector<TCHAR> buf;
	int lineLength;
	va_list args;

	va_start( args, message );

	lineLength = vsnprintf_t( NULL, 0, message.c_str(), args );
	buf.resize( lineLength + 1 );

	vsprintf_t( &buf[0], message.c_str(), args );

	if( message_queue.size() < 100 )
	{
		message_queue.push( TSTRING( &buf[0] ) );
	}
	else if ( message_queue.size() == 100 )
	{
		message_queue.push( TEXT("Additional Messages Truncated.") );
	}

	va_end( args );
}

TSTRING
ConfigLoader::PollMessages()
{
	TSTRING message = TEXT("");

	if( !message_queue.empty() )
	{
		message = message_queue.front();
		message_queue.pop();
	}

	return message;
}


ConfigLoader::~ConfigLoader()
{
	StorageMap::iterator sit;
	for ( sit = Sections.begin(); sit != Sections.end(); ++sit )
	{
		delete sit->second;
	}
	Sections.clear();
}


bool
ConfigLoader::AddSection(ParserBase* section)
{
	bool retrn = false;
	TSTRING name( section->section_name );

	/* section headers are case insensitive */
	std::transform( name.begin(), name.end(), name.begin(), ::toupper );
	ParserBase* newBase = GetSection( name );

	if ( newBase == nullptr )
	{
		std::vector<TSTRING> sectionMap;
		if ( FileMap.count( name ) != 0 )
		{
			sectionMap = FileMap[name];

			/* parse the existing section using the new parser */
			TSTRING key;
			TSTRING value;
			TSTRING line;
			TSTRING::size_type index;
			for ( unsigned int i = 0; i < sectionMap.size(); ++i )
			{
				line = sectionMap[i];
				index = line.find( '=' );
				/* Section is designed to use indexing ( key, value ), but if there is
				 * no key found, then an 'auto-key' will be generated. */
				if( index != TSTRING::npos )
				{
					key = util::trim( line.substr( 0, index ) );
					value = util::trim( line.substr( index + 1 ) );
				}
				else
				{
					key = util::Int64ToString( ++section->auto_key );
					value = util::trim( line );
				}
				section->Parse( key, value );
			}
			Sections[name] = section;
			if ( section->auto_key > 0 )
			{
				AddMessage( TEXT("AutoKeys Generated for section %s: %d"), section->section_name.c_str(), section->auto_key );
			}
			retrn = true;
		}
	}
	return retrn;
}

void
ConfigLoader::DeleteSection( const TSTRING& section_name )
{
	/* Not currently implemented, ( never been needed ). */
}


ParserBase*
ConfigLoader::GetFileSection( const TSTRING& file, const TSTRING& section )
{
	TSTRING sanitised = RemoveExtension( file );
	if( OpenConfigs.count( sanitised ) )
	{
		return OpenConfigs[sanitised]->GetSection( section );
	}
	return nullptr;
}


ParserBase*
ConfigLoader::GetSection( const TSTRING& section_name )
{
	TSTRING name( section_name );
	/* section headers are case insensitive */
	std::transform( name.begin(), name.end(), name.begin(), ::toupper );
	if( Sections.count( name ) != 0 )
	{
		return Sections[name];
	}
	return nullptr;
}


void
ConfigLoader::LoadFile()
{
	const int sMaxCmp = 512;

	FILE* hFile = nullptr;
	TSTRING value;
	std::vector<TSTRING>* sectionMap = nullptr;

	TCHAR readBuffer[sMaxCmp*2];
	TCHAR exeLocation[MAX_PATH];
	TSTRING line;
	
	TSTRING::size_type ext = fileName.rfind( '.' );
	if ( ext != TSTRING::npos )
	{
		fileType = fileName.substr( ext );
	}
	else
	{
		fileType = TEXT(".ini");
	}

	if( filePath.size() > 2 && filePath[1] != TEXT(':') )
	{
		/* if file opening fails attempt to use the exe location */
		HMODULE hm = NULL;
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
							TEXT("InitialiseConfig"),
							&hm);

		GetModuleFileName( hm, exeLocation, MAX_PATH );
		TCHAR* PathEnd = PathFindFileName( exeLocation );
		*PathEnd = TEXT('\0');

		filePath = TSTRING( exeLocation ) + filePath + TEXT('\\');
	}

	if ( fopen_t( &hFile,  TSTRING( filePath + fileName ).c_str(), TEXT("r") ) != 0 )
	{
		AddMessage( TEXT("Failed to open config file: %s"), filePath + fileName );
		return;
	}

	/* DEFAULT is now a default section that will be used if no others are avaliable */
	sectionMap = &FileMap[TEXT("DEFAULT")];

	while( EOF != fscanf_s( hFile, "%[^\n]", readBuffer, sMaxCmp*2 ) )
	{
		line.assign( readBuffer );
		fscanf_s( hFile, "%*[\n]" ); /*-- eat the newline --*/

		/*-- if this line is not a comment and not blank--*/
		if ( line[0] != ';' && line[0] )
		{
			if ( line[0] == '[' )
			{
				value = line.substr( 1, line.find( ']' ) - 1 );
				/* section headers are case insensitive */
				std::transform( value.begin(), value.end(), value.begin(), ::toupper );
				sectionMap = &FileMap[value];
			}
			else
			{
				if ( sectionMap )
				{
					sectionMap->push_back( line );
				}
				else
				{
					AddMessage( TEXT("Attempted to write to null section\n") );
				}
			}
		}
	}

	if ( hFile )
	{
		fclose( hFile );
		hFile = nullptr;
	}
}


void
ConfigLoader::CloseAll( const bool force )
{
	TSTRING::size_type i = 0;
	std::vector<TSTRING> filenames;

	ConfigMap::iterator sit;
	for ( sit = OpenConfigs.begin(); sit != OpenConfigs.end(); ++sit )
	{
		filenames.push_back( sit->second->fileName );
	}

	/* we need to do this because CloseConfig will invalidate sit when called */
	for ( i = 0; i < filenames.size(); ++i )
	{
		CloseConfig( filenames[i], true );
	}
}


void
ConfigLoader::CloseConfig(ConfigLoader* config, const bool force)
{
	CloseConfig( config->fileName, force );
}


void
ConfigLoader::CloseConfig(const TSTRING& filename, const bool force)
{
	TSTRING sanitised = RemoveExtension( filename );
	ConfigMap::iterator sit;

	if ( OpenConfigs.size() > 0 )
	{
		if( OpenConfigs.count( sanitised ) )
		{
			sit = OpenConfigs.find( sanitised );

			sit->second->references -= 1;
			if ( force )
			{
				sit->second->references = 0;
			}

			if ( sit->second->references <= 0 )
			{
				delete sit->second;
				OpenConfigs.erase( sit );
			}
		}
	}
}
