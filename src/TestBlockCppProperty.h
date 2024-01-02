#ifndef TEST_BLOCK_CPP_PROPERTY_H
#define TEST_BLOCK_CPP_PROPERTY_H
#include "BlockCpp.h"
#include "TestBase.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the property C++ block class.
 */
class Property:
    public Test::Base
{
    Q_OBJECT
    ::Block::Cpp::Property* _block {nullptr};


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the display icon property.
     */
    private slots:
    void displayIconProperty(
    );


    /*!
     * Tests the "is abstract" method.
     */
    private slots:
    void isAbstract(
    );


    /*!
     * Tests the "is virtual" method.
     */
    private slots:
    void isVirtual(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}
}


#endif
