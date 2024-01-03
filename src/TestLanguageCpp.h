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
     * Tests creation of an exception item block.
     */
    private slots:
    void createException(
    );


    /*!
     * Tests creation of a function block.
     */
    private slots:
    void createFunction(
    );


    /*!
     * Tests creation of a namespace block.
     */
    private slots:
    void createNamespace(
    );


    /*!
     * Tests creation of a property block.
     */
    private slots:
    void createProperty(
    );


    /*!
     * Tests creation of a union block.
     */
    private slots:
    void createUnion(
    );


    /*!
     * Tests creation of a variable block.
     */
    private slots:
    void createVariable(
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
