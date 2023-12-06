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
     * Tests the import static method with this project's own C++/Qt project
     * file.
     * 
     * TODO: The project model needs to be refactored to have a import static
     * method, right now its constructor is being used.
     * 
     * TODO: Eventually a project should be generated instead of using this
     * project's own SRP file.
     */
    private slots:
    void importCppQt(
    );
};
}
}


#endif
