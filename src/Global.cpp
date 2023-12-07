#include "Global.h"
#include <QtWidgets>
#include "Test.h"


int main(
    int argc
    ,char** argv
)
{
    QApplication::setApplicationName("Socrates' Reference");
    QApplication::setApplicationVersion("1.0.9999");
    if (
        argc >= 2
        && !strcmp(argv[1],"--test")
    )
    {
        return Test::execute(argc,argv);
    }
    QApplication application(argc,argv);
    return application.exec();
}
