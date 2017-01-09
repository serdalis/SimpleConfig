[![Build Status](https://travis-ci.org/serdalis/SimpleConfig.svg?branch=master)](https://travis-ci.org/serdalis/SimpleConfig)
[![Coverage Status](https://coveralls.io/repos/github/serdalis/SimpleConfig/badge.svg?branch=master)](https://coveralls.io/github/serdalis/SimpleConfig?branch=master)

---

# SimpleConfig
Lite weight configuration file management. Designed for large projects which require one or more configuration files.
Allows for custom configuration parsing and usage.

## Example Usage

### Simple Usage Example

```C++
#include "config_loader.h"

int
main( int argc, char* argv[] )
{
	/* open the configuration file, or get an existing instance. */
	CONFIGHANDLE config = OPEN_CONFIG( TEXT( "test.ini" ) );

	/* define a parser that will be used with this configuration file. */
	DefaultParser* test1 = (DefaultParser*)config->GetSection( TEXT( "test1" ) );

	/* if the parser was not successfully obtained */
	if ( !test1 )
	{
		/* this next section of code is to prevent crashes with un-defined parsers or
		 * to allow program internal configuration parsers to exist. */

		/* create a new section for the configuration file. */
		test1 = new DefaultParser( TEXT( "test1" ) );
		/* attempt to add the section to the configuration file. */
		if ( !config->AddSection( test1 ) )
		{
			delete test1;
		}
	}

	/* config handle will auto close the configuration file when it is out of scope. */
	return 0;
}
```

### Example Custom Parser

```C++
/**
* Structure that will hold the information that has been parsed by the config loader.
*/
struct TcpNode {
public:
	/**
	* Default Constructor, does nothing.
	*/
	TcpNode() : _port( 0 ) {};

	/**
	* This operator is nessisary to assign values to the map if a complete object is stored.
	* @param rhs right hand side object that will have its values copied into this one.
	* @return const reference to the called obejct.
	*/
	const TcpNode& operator=( const TcpNode& rhs )
	{
		this->_ip = rhs._ip;
		this->_sip = rhs._sip;
		this->_port = rhs._port;
		return *this;
	}

	/**
	* Constructor
	* @param Ip String containing the Ip Address.
	* @param Sip String contraining the Source Ip Address.
	* @param Port Port number to remotely connect to.
	*/
	TcpNode( TSTRING Ip, TSTRING Sip, unsigned short Port )
		: _ip( Ip ), _sip( Sip ), _port( Port ) {}

	TSTRING _ip; /**< Remote Ip Address. */
	TSTRING _sip; /**< Source Ip Address. */
	unsigned short _port; /**< Remote Port */
};

/**
* Processes Tcp Nodes from a config file.
* Dispatch will use this parser to get Nodes based on configuration data.
* Will return structures strings based off config data.
*/
class TcpNodeParser : public Parser<TcpNode>
{
public:
	/**
	* Constructor, does nothing except call base constructor.
	* @param sectionName name of section in config file to wrap this parser around.
	*/
	TcpNodeParser( const TSTRING& sectionName )
		: Parser( sectionName ) {};

	/**
	* Overwritten Parse function that will take a key / value pair and process it
	* in the way the custom parser specifies.
	* @param key Key to store the custom structure under.
	* @param value
	*/
	void Parse( const TSTRING& key, const TSTRING& value )
	{
		/* define your own logic here */
		if ( Configuration.count( key ) == 0 )
		{
			Configuration[key] = TcpNode( /* parsed values from string */ );
		}
		else
		{
			message = TEXT( "Duplicate Configuration Key: %s\n" ) + key;
		}
	}

	/**
	* Returns a format string based on the key and format type.
	* @param key key to use when looking up the fomrat string.
	* @return string with format compatable with type specified.
	*/
	const TcpNode getTcpNode( const TSTRING& key )
	{
		MapType::iterator titer;
		titer = (Configuration.size() > 0) ? Configuration.find( key ) : Configuration.end();
		if ( titer != Configuration.end() ) {
			return titer->second;
		}
		message = TEXT( "Failed to get key %s from Tcp Config\n" ) + key;
		return TcpNode();
	}

	/**
	* Destructor, deletes all elements in the internal map if nessisary.
	*/
	virtual ~TcpNodeParser();
};
```
