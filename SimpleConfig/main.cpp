#include "config_loader.h"


static CONFIGHANDLE config;


int
main( int argc, char* argv[] )
{
	int error = 0;

	config = OPEN_CONFIG( TEXT("test.ini") );

	return error;
}