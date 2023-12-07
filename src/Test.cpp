#include "Test.h"
#include <QtTest>
#include "TestBlockCppClass.h"
#include "TestBlockCppEnumeration.h"
#include "TestBlockCppNamespace.h"
#include "TestLanguageCpp.h"
#include "TestLanguageCppQt.h"
#include "TestModelProject.h"
namespace Test {


int execute(
    int argc
    ,char** argv
)
{
    struct TestObject
    {
        QString name;
        QObject* ptr;
    };
    static const TestObject tests[] = {
        {"BlockCppClass",new Block::Cpp::Class}
        ,{"BlockCppEnumeration",new Block::Cpp::Enumeration}
        ,{"BlockCppNamespace",new Block::Cpp::Namespace}
        ,{"LanguageCpp",new Language::Cpp}
        ,{"LanguageCppQt",new Language::CppQt}
        ,{"ModelProject",new Model::Project}
    };
    auto printTestList = [](QTextStream& out) {
        out << QApplication::translate("main","Available unit tests:\n");
        for (const auto& test: tests)
        {
            out << test.name << "\n";
        }
    };
    if (argc < 3)
    {
        QTextStream out(stdout);
        printTestList(out);
        return 0;
    }
    QString testName = argv[2];
    QStringList arguments;
    for (int i = 2;i < argc;i++)
    {
        arguments += argv[i];
    }
    for (const auto& test: tests)
    {
        if (test.name == testName)
        {
            return QTest::qExec(test.ptr,arguments);
        }
    }
    QTextStream out(stdout);
    auto msg = QApplication::translate("main","%1: Unknown unit test '%2'.\n");
    msg = msg.arg(QApplication::applicationName(),testName);
    out << msg;
    printTestList(out);
    return 1;
}
}
