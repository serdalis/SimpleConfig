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