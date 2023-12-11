#ifndef TEST_FACTORY_LANGUAGE_H
#define TEST_FACTORY_LANGUAGE_H
#include <QObject>
#include "Factory.h"
namespace Test {
namespace Factory {




/*!
 * This is a Qt unit test class. It tests the language factory singleton class.
 */
class Language:
    public QObject
{
    Q_OBJECT
    ::Factory::Language* _factory;
    int _cppIndex;
    int _cppQtIndex;


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the get method.
     */
    private slots:
    void get(
    );


    /*!
     * Tests the meta method.
     */
    private slots:
    void meta(
    );


    /*!
     * Tests the size method.
     */
    private slots:
    void size(
    );
};
}
}


#endif
