#ifndef TEST_BLOCK_CPP_NAMESPACE_H
#define TEST_BLOCK_CPP_NAMESPACE_H
#include "BlockCpp.h"
#include "TestBase.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the namespace C++ block class.
 */
class Namespace:
    public Test::Base
{
    Q_OBJECT
    ::Block::Cpp::Namespace* _block;


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the description property.
     */
    private slots:
    void descriptionProperty(
    );


    /*!
     * Tests the display icon property.
     */
    private slots:
    void displayIconProperty(
    );


    /*!
     * Tests the display text property.
     */
    private slots:
    void displayTextProperty(
    );


    /*!
     * Tests the load from map interface.
     */
    private slots:
    void loadFromMap(
    );


    /*!
     * Tests the name property.
     */
    private slots:
    void nameProperty(
    );


    /*!
     * Tests the save to map interface.
     */
    private slots:
    void saveToMap(
    );


    /*!
     * Tests the scope property.
     */
    private slots:
    void scopeProperty(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}
}


#endif
