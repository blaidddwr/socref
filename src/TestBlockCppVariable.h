#ifndef TEST_BLOCK_CPP_VARIABLE_H
#define TEST_BLOCK_CPP_VARIABLE_H
#include "BlockCpp.h"
#include "TestBase.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the variable C++ block class.
 */
class Variable:
    public Test::Base
{
    Q_OBJECT
    ::Block::Cpp::Variable* _block {nullptr};


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the type property.
     */
    private slots:
    void assignmentProperty(
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


    /*!
     * Tests the type property.
     */
    private slots:
    void typeProperty(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}
}


#endif
