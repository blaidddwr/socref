#ifndef TEST_BLOCK_CPP_FUNCTION_H
#define TEST_BLOCK_CPP_FUNCTION_H
#include "BlockCpp.h"
#include "TestBase.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the function C++ block class.
 */
class Function:
    public Test::Base
{
    Q_OBJECT
    ::Block::Cpp::Function* _block;


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the access property.
     */
    private slots:
    void accessProperty(
    );


    /*!
     * Tests the assignment property.
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
     * Tests the display text property.
     */
    private slots:
    void displayTextProperty(
    );


    /*!
     * Tests the flags property.
     */
    private slots:
    void flagsProperty(
    );


    /*!
     * Tests the "is abstract" method.
     */
    private slots:
    void isAbstract(
    );


    /*!
     * Tests the "is constant" method.
     */
    private slots:
    void isConstant(
    );


    /*!
     * Test the "is constructor" method.
     */
    private slots:
    void isConstructor(
    );


    /*!
     * Tests the "is default" method.
     */
    private slots:
    void isDefault(
    );


    /*!
     * Tests the "is deleted" method.
     */
    private slots:
    void isDeleted(
    );


    /*!
     * Tests the "is destructor" method.
     */
    private slots:
    void isDestructor(
    );


    /*!
     * Tests the "is explicit" method.
     */
    private slots:
    void isExplicit(
    );


    /*!
     * Tests the "is final" method.
     */
    private slots:
    void isFinal(
    );


    /*!
     * Tests the "is method" method.
     */
    private slots:
    void isMethod(
    );


    /*!
     * Tests the "is no exception" method.
     */
    private slots:
    void isNoExcept(
    );


    /*!
     * Tests the "is operator" method.
     */
    private slots:
    void isOperator(
    );


    /*!
     * Tests the "is override" method.
     */
    private slots:
    void isOverride(
    );


    /*!
     * Tests the "is private" method.
     */
    private slots:
    void isPrivate(
    );


    /*!
     * Tests the "is protected" method.
     */
    private slots:
    void isProtected(
    );


    /*!
     * Tests the "is public" method.
     */
    private slots:
    void isPublic(
    );


    /*!
     * Tests the "is static" method.
     */
    private slots:
    void isStatic(
    );


    /*!
     * Tests the "is virtual" method.
     */
    private slots:
    void isVirtual(
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
     * Tests the return description property.
     */
    private slots:
    void returnDescriptionProperty(
    );


    /*!
     * Tests the return type property.
     */
    private slots:
    void returnTypeProperty(
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
