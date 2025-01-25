#ifndef TEST_MODEL_PROJECT_H
#define TEST_MODEL_PROJECT_H
#include <QObject>
namespace Test {
namespace Model {




/*!
 * This is a Qt unit test class. It tests the project model class.
 */
class Project:
    public QObject
{
    Q_OBJECT


    /*!
     * Tests the to and from directory methods from the project stream class
     * using the current format.
     */
    private slots:
    void toDirFromDir(
    );


    /*!
     * Tests the to and from XML methods from the project stream class using the
     * current format.
     */
    private slots:
    void toXmlFromXml(
    );
};
}
}


#endif
