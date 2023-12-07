#ifndef TEST_LANGUAGE_CPP_H
#define TEST_LANGUAGE_CPP_H
#include <QObject>
#include "Language.h"
#include "ModelMeta.h"
namespace Test {
namespace Language {




/*!
 * This is a Qt unit test class. It tests the C++ language class.
 */
class Cpp:
    public QObject
{
    Q_OBJECT
    Model::Meta::Language* _meta;
    ::Language::Cpp* _language;


    private slots:
    void initTestCase(
    );


    /*!
     * Tests creation of a class block.
     */
    private slots:
    void createClass(
    );


    /*!
     * Tests creation of an enumeration block.
     */
    private slots:
    void createEnumeration(
    );


    /*!
     * Tests creation of a namespace block.
     */
    private slots:
    void createNamespace(
    );


    /*!
     * Tests the root index interface.
     */
    private slots:
    void rootIndex(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}


#endif
