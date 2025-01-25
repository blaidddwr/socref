#ifndef TEST_BLOCK_CPP_CLASS_H
#define TEST_BLOCK_CPP_CLASS_H
#include <QObject>
#include "BlockCpp.h"
#include "Language.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the class C++ block class.
 */
class Class:
    public QObject
{
    Q_OBJECT
    ::Block::Cpp::Class* _block;
    Language::Abstract* _language {nullptr};


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
