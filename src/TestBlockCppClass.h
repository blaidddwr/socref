#ifndef TEST_BLOCK_CPP_CLASS_H
#define TEST_BLOCK_CPP_CLASS_H
#include "BlockCpp.h"
#include "TestBase.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the class C++ block class.
 */
class Class:
    public Test::Base
{
    Q_OBJECT
    ::Block::Cpp::Class* _block;


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
     * Tests the load from map interface.
     */
    private slots:
    void loadFromMap(
    );


    /*!
     * Tests the load from map interface using the legacy version.
     */
    private slots:
    void loadFromMapLegacy(
    );


    /*!
     * Tests the parents property.
     */
    private slots:
    void parentsProperty(
    );


    /*!
     * Tests the save to map interface.
     */
    private slots:
    void saveToMap(
    );


    /*!
     * Tests the templates property.
     */
    private slots:
    void templatesProperty(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}
}


#endif
