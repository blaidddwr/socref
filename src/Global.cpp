#include "Global.h"
#include <QtWidgets>


int main(
    int argc
    ,char** argv
)
{
    QApplication application(argc,argv);
    QApplication::setApplicationName("Socrates' Reference");
    QApplication::setApplicationVersion("1.0.0");
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
        //TODO: unit test
        return 0;
    }
}
