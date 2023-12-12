#ifndef TEST_MODEL_PROJECT_H
#define TEST_MODEL_PROJECT_H
#include "TestBase.h"
namespace Test {
namespace Model {




/*!
 * This is a Qt unit test class. It tests the project model class.
 */
class Project:
    public Test::Base
{
    Q_OBJECT


    private slots:
    void initTestCase(
    );


    /*!
     * Tests the import method using the legacy format.
     */
    private slots:
    void importLegacy(
    );


    /*!
     * Tests the load constructor.
     */
    private slots:
    void loadConstruct(
    );


    private slots:
    void cleanupTestCase(
    );
};
}
}


#endif
