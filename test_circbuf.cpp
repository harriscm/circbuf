#include <stdio.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "circbuf.h"

#include <cppunit/extensions/HelperMacros.h>

constexpr unsigned char testString[100] = "wEq6ScPu1O64pjWNHbbc8NqwUBK2wKKfEjLyvAT04iLyPsOK1ePTlwrGB1tTeYAcQVlJT4cKcVw5sGWimEjX9D4Sg5efuPUYTNb";

class CircbufTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CircbufTest);
    CPPUNIT_TEST(testCircbufInit);
    CPPUNIT_TEST(testCircbufPut);

    CPPUNIT_TEST_SUITE_END();

    unsigned char buffer[1024];
    circbuf_t test;

    public:
        void setUp(){
            memset(buffer, 0, 1024);
            circbuf_init(&test, buffer, 1024);
        }

        void teardown(){}

        void testCircbufPut()
        {
            unsigned char singleWrite = 0xA5;
            
            CPPUNIT_ASSERT(test.put(NULL,&singleWrite, 1));
            CPPUNIT_ASSERT(test.put(&test, NULL, 1));
            CPPUNIT_ASSERT(test.put(&test, &singleWrite, 0));


            CPPUNIT_ASSERT(!test.put(&test, &singleWrite, 1));
            CPPUNIT_ASSERT(buffer[0] == 0xA5);

            CPPUNIT_ASSERT(!test.put(&test, testString, 100));
            CPPUNIT_ASSERT(buffer[101] == '\0');
            
        }

        void testCircbufInit()
        {
            circbuf_t buf = {0}; 
            CPPUNIT_ASSERT_EQUAL(1, circbuf_init(NULL, buffer, 1024));
            CPPUNIT_ASSERT_EQUAL(1, circbuf_init(&buf, NULL, 1024));
            CPPUNIT_ASSERT_EQUAL(1, circbuf_init(&buf, buffer, 0));
            CPPUNIT_ASSERT_EQUAL(0, circbuf_init(&buf, buffer, 1024));
            if(!buf.reset) CPPUNIT_FAIL("Reset method not returned");
            if(!buf.size) CPPUNIT_FAIL("Size method not returned");
            if(!buf.capacity) CPPUNIT_FAIL("Capacity method not returned");
            if(!buf.put) CPPUNIT_FAIL("Put method not returned");
            if(!buf.try_put) CPPUNIT_FAIL("try_put method not returned");
            if(!buf.get) CPPUNIT_FAIL("get method not returned");
            if(!buf.peek) CPPUNIT_FAIL("peek method not returned");
            if(!buf.empty) CPPUNIT_FAIL("empty method not returned");
            if(!buf.full) CPPUNIT_FAIL("full method not returned");

            CPPUNIT_ASSERT(buffer == buf.buffer);
            CPPUNIT_ASSERT(buf.head == 0);
            CPPUNIT_ASSERT(buf.tail == 0);
            CPPUNIT_ASSERT(buf.max == 1024);
            CPPUNIT_ASSERT(!buf.isFull);
        }

};

CPPUNIT_TEST_SUITE_REGISTRATION(CircbufTest);

int main(int argc, char **argv)
{
    // Get the top level suite from the registry
  CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  // Adds the test to the list of test to run
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( suite );

  // Change the default outputter to a compiler error format outputter
  runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(),
                                                       std::cerr ) );
  // Run the tests.
  bool wasSucessful = runner.run();

  // Return error code 1 if the one of test failed.
  return wasSucessful ? 0 : 1;
}