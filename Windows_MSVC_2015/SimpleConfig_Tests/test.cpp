#include "stdafx.h"
#include "CppUnitTest.h"

#include "config_loader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SimpleConfig_Tests
{
	TEST_CLASS( ConfigFile_Test )
	{
	public:
		TEST_METHOD( LoadFile_Test )
		{

		}
	};

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

		TEST_METHOD( DefaultParser_Parse )
		{
			DefaultParser testParser( TEXT( "TestSection" ) );
			
			/* check that the duplicate message is not sent on successful parse. */
			testParser.Parse( TEXT( "TestKey" ), TEXT( "1" ) );
			Assert::AreEqual( TSTRING( TEXT( "" ) ), testParser.CheckMessage() );

			/* check that the item was actually inserted into the mapping. */
			Assert::AreEqual( TSTRING( TEXT( "1" ) ), testParser.getString( TEXT( "TestKey" ), TEXT( "NULL" ) ) );

			/* check that the duplicate message is send on failed parse. */
			testParser.Parse( TEXT( "TestKey" ), TEXT( "1" ) );
			Assert::AreEqual( TSTRING( TEXT( "Duplicate Configuration Key: TestKey" ) ), testParser.CheckMessage() );

			/* test entering a few values for the mapping. */
			testParser.Parse( TEXT( "TestKey2" ), TEXT( "2" ) );
			testParser.Parse( TEXT( "TestKey3" ), TEXT( "3" ) );
			testParser.Parse( TEXT( "TestKey4" ), TEXT( "4" ) );
			testParser.Parse( TEXT( "TestKey5" ), TEXT( "5" ) );
			testParser.Parse( TEXT( "TestKey6" ), TEXT( "6" ) );
			testParser.Parse( TEXT( "TestKey7" ), TEXT( "7" ) );

			/* see if they are all mapped correctly */
			Assert::AreEqual( TSTRING( TEXT( "1" ) ), testParser.getString( TEXT( "TestKey" ), TEXT( "" ) ) );
			Assert::AreEqual( TSTRING( TEXT( "2" ) ), testParser.getString( TEXT( "TestKey2" ), TEXT( "" ) ) );
			Assert::AreEqual( TSTRING( TEXT( "3" ) ), testParser.getString( TEXT( "TestKey3" ), TEXT( "" ) ) );
			Assert::AreEqual( TSTRING( TEXT( "4" ) ), testParser.getString( TEXT( "TestKey4" ), TEXT( "" ) ) );
			Assert::AreEqual( TSTRING( TEXT( "5" ) ), testParser.getString( TEXT( "TestKey5" ), TEXT( "" ) ) );
			Assert::AreEqual( TSTRING( TEXT( "6" ) ), testParser.getString( TEXT( "TestKey6" ), TEXT( "" ) ) );
			Assert::AreEqual( TSTRING( TEXT( "7" ) ), testParser.getString( TEXT( "TestKey7" ), TEXT( "" ) ) );
		}

		TEST_METHOD( DefaultParser_getString )
		{
			DefaultParser testParser( TEXT( "TestSection" ) );
		}

		TEST_METHOD( DefaultParser_getInt16 )
		{
			DefaultParser testParser( TEXT( "TestSection" ) );
		}

		TEST_METHOD( DefaultParser_getInt32 )
		{
			DefaultParser testParser( TEXT( "TestSection" ) );
		}

		TEST_METHOD( DefaultParser_getInt64 )
		{
			DefaultParser testParser( TEXT( "TestSection" ) );
		}

		TEST_METHOD( DefaultParser_getDouble )
		{
			DefaultParser testParser( TEXT( "TestSection" ) );
		}

	};
}