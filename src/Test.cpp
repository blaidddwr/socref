#include "Test.h"
#include <QtTest>
#include "TestModelProject.h"
namespace Test {


int execute(
    int argc
    ,char** argv
)
{
    for (int i = 0;i < argc;i++)
    {
        if (!strcmp("--test",argv[i]))
        {
            std::swap(argv[i],argv[argc-1]);
            argc--;
            break;
        }
    }
    int ret = 0;
    ret |= QTest::qExec(new Test::Model::Project,argc,argv);
    return ret;
}
}
