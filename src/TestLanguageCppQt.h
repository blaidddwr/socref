#ifndef TEST_LANGUAGE_CPPQT_H
#define TEST_LANGUAGE_CPPQT_H
#include <QObject>
#include "Language.h"
#include "ModelMeta.h"
namespace Test {
namespace Language {




/*!
 * This is a Qt unit test class. It tests the C++/Qt language class.
 */
class CppQt:
    public QObject
{
    Q_OBJECT
    Model::Meta::Language* _meta;
    ::Language::CppQt* _language;


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
