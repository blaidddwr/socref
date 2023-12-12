#ifndef TEST_BLOCK_ABSTRACT_H
#define TEST_BLOCK_ABSTRACT_H
#include "BlockTest.h"
#include "ModelMeta.h"
#include "TestBase.h"
class QDir;
namespace Test {
namespace Block {




/*!
 * This is a Qt unit test class. It tests the abstract block class.
 */
class Abstract:
    public Test::Base
{
    Q_OBJECT
    ::Block::Test::Node* _block;
    Model::Meta::Language* _meta;


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the append method.
     */
    private slots:
    void append(
    );


    /*!
     * Tests the descendants method.
     */
    private slots:
    void descendants(
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
     * Tests the from directory method.
     */
    private slots:
    void fromDir(
    );


    /*!
     * Tests the from XML method.
     */
    private slots:
    void fromXml(
    );


    /*!
     * Tests the from XML method using the legacy format.
     */
    private slots:
    void fromXmlLegacy(
    );


    /*!
     * Tests the get method.
     */
    private slots:
    void get(
    );


    /*!
     * Tests the index of method.
     */
    private slots:
    void indexOf(
    );


    /*!
     * Tests the insert method.
     */
    private slots:
    void insert(
    );


    /*!
     * Tests the meta property.
     */
    private slots:
    void metaProperty(
    );


    /*!
     * Tests the move method.
     */
    private slots:
    void move(
    );


    /*!
     * Tests the scope property.
     */
    private slots:
    void scopeProperty(
    );


    /*!
     * Tests the size property.
     */
    private slots:
    void size(
    );


    /*!
     * Tests the take method.
     */
    private slots:
    void take(
    );


    /*!
     * Tests the to directory method.
     */
    private slots:
    void toDir(
    );


    /*!
     * Tests the to XML method.
     */
    private slots:
    void toXml(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}


#endif
