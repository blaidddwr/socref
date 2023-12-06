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
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(
        {
            "test"
            ,QApplication::translate(
                "main"
                ,"Run all unit tests for this application and then exit."
            )
        }
    );
    parser.process(application);
    if (!parser.isSet("test"))
    {
        return application.exec();
    }
    else
    {
        return Test::execute(argc,argv);
    }
}
