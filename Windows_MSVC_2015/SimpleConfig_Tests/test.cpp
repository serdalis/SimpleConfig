#include "stdafx.h"
#include "CppUnitTest.h"

#include "config_loader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SimpleConfig_Tests
{		
	TEST_CLASS( DefaultParser_Test )
	{
	public:
		
		TEST_METHOD( DefaultParser_CheckMessage )
		{
			/* check that the message is empty by default */
			DefaultParser testParser( TEXT( "TestSection" ) );
			Assert::AreEqual( TSTRING( TEXT( "" ) ), testParser.CheckMessage() );

			/* check that the duplicate message is not sent on successful parse. */
			testParser.Parse( TEXT( "TestKey" ), TEXT( "1" ) );
			Assert::AreEqual( TSTRING( TEXT( "" ) ), testParser.CheckMessage() );

			/* check that the duplicate message is send on failed parse. */
			testParser.Parse( TEXT( "TestKey" ), TEXT( "1" ) );
			Assert::AreEqual( TSTRING( TEXT( "Duplicate Configuration Key: TestKey" ) ), testParser.CheckMessage() );
		}

	};
}