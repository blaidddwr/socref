#ifndef TEST_BLOCK_CPP_VARIABLE_H
#define TEST_BLOCK_CPP_VARIABLE_H
#include <QObject>
#include "BlockCpp.h"
#include "Language.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the variable C++ block class.
 */
class Variable:
    public QObject
{
    Q_OBJECT
    ::Block::Cpp::Variable* _block {nullptr};
    Language::Abstract* _language {nullptr};


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
     * Tests the "set state" interface.
     */
    private slots:
    void setState(
    );


    /*!
     * Tests the state interface.
     */
    private slots:
    void state(
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
