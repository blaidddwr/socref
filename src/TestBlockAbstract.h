#ifndef TEST_BLOCK_ABSTRACT_H
#define TEST_BLOCK_ABSTRACT_H
#include <QObject>
#include "Block.h"
#include "Language.h"
#include "ModelMeta.h"
class QDir;
namespace Test {
namespace Block {




/*!
 * This is a Qt unit test class. It tests the abstract block class.
 */
class Abstract:
    public QObject
{
    Q_OBJECT
    ::Block::Abstract* _block {nullptr};
    Language::Abstract* _language {nullptr};
    Model::Meta::Block* _blockMeta {nullptr};
    Model::Meta::Language* _languageMeta {nullptr};


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
     * Tests the to and from directory methods from the block stream class using
     * the current format.
     */
    private slots:
    void toDirFromDir(
    );


    /*!
     * Tests the to and from XML methods from the block stream class using the
     * current format.
     */
    private slots:
    void toXmlFromXml(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}


#endif
