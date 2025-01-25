#ifndef TEST_BLOCK_CPP_EXCEPTIONITEM_H
#define TEST_BLOCK_CPP_EXCEPTIONITEM_H
#include <QObject>
#include "BlockCpp.h"
namespace Test {
namespace Block {
namespace Cpp {




/*!
 * This is a Qt unit test class. It tests the exception item C++ block class.
 */
class ExceptionItem:
    public QObject
{
    Q_OBJECT
    ::Block::Cpp::Exception* _block {nullptr};


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the display icon property.
     */
    private slots:
    void displayIconProperty(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}
}


#endif
