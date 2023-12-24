#ifndef TEST_BLOCK_CPP_EXCEPTION_H
#define TEST_BLOCK_CPP_EXCEPTION_H
#include "BlockCpp.h"
#include "TestBase.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the exception C++ block class.
 */
class Exception:
    public Test::Base
{
    Q_OBJECT
    ::Block::Cpp::Exception* _block {nullptr};


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the display icon property.
     */
    private slots:
    void displayIconProperty(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}
}


#endif
