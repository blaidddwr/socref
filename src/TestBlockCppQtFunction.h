#ifndef TEST_BLOCK_CPPQT_FUNCTION_H
#define TEST_BLOCK_CPPQT_FUNCTION_H
#include "BlockCppQt.h"
#include "TestBase.h"
namespace Test {
namespace Block {
namespace CppQt {




/*!
 * This is a Qt unit test class. It tests the function C++/Qt block class.
 */
class Function:
    public Test::Base
{
    Q_OBJECT
    ::Block::Cpp::Class* _parent {nullptr};
    ::Block::CppQt::Function* _block {nullptr};


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
     * Tests the display text property.
     */
    private slots:
    void displayTextProperty(
    );


    /*!
     * Tests the "is Qt invokable" method.
     */
    private slots:
    void isQtInvokable(
    );


    /*!
     * Tests the "is signal" method.
     */
    private slots:
    void isSignal(
    );


    /*!
     * Tests the "is slot" method.
     */
    private slots:
    void isSlot(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}
}


#endif
