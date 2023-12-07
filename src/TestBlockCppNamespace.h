#ifndef TEST_BLOCK_CPP_NAMESPACE_H
#define TEST_BLOCK_CPP_NAMESPACE_H
#include <QObject>
#include "BlockCpp.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the namespace C++ block class.
 */
class Namespace:
    public QObject
{
    Q_OBJECT
    ::Block::Cpp::Namespace* _block;


    private slots:
    void initTestCase(
    );


    /*!
     * Creates and returns a new namespace C++ block. The returned block's
     * parent is this unit test instance.
     */
    private:
    ::Block::Cpp::Namespace* create(
    );


    /*!
     * Tests the description property.
     */
    private slots:
    void descriptionProperty(
    );


    /*!
     * Tests the display text property.
     */
    private slots:
    void displayText(
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


    private slots:
    void cleanupTestCase(
    );
};
}
}
}


#endif
