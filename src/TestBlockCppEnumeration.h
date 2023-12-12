#ifndef TEST_BLOCK_CPP_ENUMERATION_H
#define TEST_BLOCK_CPP_ENUMERATION_H
#include "BlockCpp.h"
#include "TestBase.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the enumeration C++ block class.
 */
class Enumeration:
    public Test::Base
{
    Q_OBJECT
    ::Block::Cpp::Enumeration* _block;
    int _blockIndex;


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the class property.
     */
    private slots:
    void classProperty(
    );


    /*!
     * Tests the display icon property.
     */
    private slots:
    void displayIconProperty(
    );


    /*!
     * Tests the load from map interface.
     */
    private slots:
    void loadFromMap(
    );


    /*!
     * Tests the save to map interface.
     */
    private slots:
    void saveToMap(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}
}


#endif
