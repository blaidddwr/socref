#ifndef TEST_BLOCK_CPP_ENUMERATION_H
#define TEST_BLOCK_CPP_ENUMERATION_H
#include <QObject>
#include "BlockCpp.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the enumeration C++ block class.
 */
class Enumeration:
    public QObject
{
    Q_OBJECT
    ::Block::Cpp::Enumeration* _block;


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


    /*!
     * Creates and returns a new enumeration C++ block. The returned block's
     * parent is this unit test instance.
     */
    private:
    ::Block::Cpp::Enumeration* create(
    );
};
}
}
}


#endif
