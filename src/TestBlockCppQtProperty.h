#ifndef TEST_BLOCK_CPPQT_PROPERTY_H
#define TEST_BLOCK_CPPQT_PROPERTY_H
#include "BlockCppQt.h"
#include "TestBase.h"
namespace Test {
namespace Block {
namespace CppQt {




/*!
 * This is a Qt unit test class. It tests the property C++/Qt block class.
 */
class Property:
    public Test::Base
{
    Q_OBJECT
    ::Block::CppQt::Property* _block {nullptr};


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the notify signal property.
     */
    private slots:
    void notifySignal(
    );


    /*!
     * Tests the read method property.
     */
    private slots:
    void readMethod(
    );


    /*!
     * Tests the write method property.
     */
    private slots:
    void writeMethod(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}
}


#endif
