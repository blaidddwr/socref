#include "Global.h"
#include <QtWidgets>
#include "Test.h"


int main(
    int argc
    ,char** argv
)
{
    QApplication application(argc,argv);
    QApplication::setApplicationName("Socrates' Reference");
    QApplication::setApplicationVersion("1.0.9999");
    if (argc >= 2)
    {
        if (!strcmp(argv[1],"--test"))
        {
            Test::initialize();
            return Test::execute(argc,argv);
        }
        else if (!strcmp(argv[1],"--test-all"))
        {
            Test::initialize();
            return Test::executeAll(argc,argv);
        }
    }
    return application.exec();
}
