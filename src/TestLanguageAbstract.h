#ifndef TEST_LANGUAGE_ABSTRACT_H
#define TEST_LANGUAGE_ABSTRACT_H
#include <QObject>
#include "Language.h"
#include "ModelMeta.h"
namespace Test {
namespace Language {




/*!
 * This is a Qt unit test class. It tests the abstract language class by using
 * the test language class.
 */
class Abstract:
    public QObject
{
    Q_OBJECT
    ::Language::Abstract* _language;
    Model::Meta::Language* _meta;


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the block meta method.
     */
    private slots:
    void blockMeta(
    );


    /*!
     * Tests the index from name method.
     */
    private slots:
    void indexFromName(
    );


    /*!
     * Tests the meta property.
     */
    private slots:
    void metaProperty(
    );


    /*!
     * Test the size method.
     */
    private slots:
    void size(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}


#endif
