#include "Test.h"
#include <QtTest>
#include "Exception.h"
#include "TestBlockAbstract.h"
#include "TestBlockCppClass.h"
#include "TestBlockCppEnumeration.h"
#include "TestBlockCppExceptionItem.h"
#include "TestBlockCppFunction.h"
#include "TestBlockCppNamespace.h"
#include "TestBlockCppProperty.h"
#include "TestBlockCppQtFunction.h"
#include "TestBlockCppQtProperty.h"
#include "TestBlockCppVariable.h"
#include "TestFactoryLanguage.h"
#include "TestLanguageAbstract.h"
#include "TestLanguageCpp.h"
#include "TestLanguageCppQt.h"
#include "TestModelProject.h"
namespace Test {
struct TestObject
{
    QString name;
    QObject* ptr;
};
const QList<TestObject>* _g_tests {nullptr};


int execute(
    int argc
    ,char** argv
)
{
    G_ASSERT(_g_tests);
    auto printTestList = [](QTextStream& out) {
        out << QApplication::translate("main","Available unit tests:\n");
        for (const auto& test: *_g_tests)
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
    QStringList arguments {argv[0]};
    for (int i = 3;i < argc;i++)
    {
        arguments += argv[i];
    }
    for (const auto& test: *_g_tests)
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


int executeAll(
    int argc
    ,char** argv
)
{
    G_ASSERT(_g_tests);
    QStringList arguments;
    for (int i = 2;i < argc;i++)
    {
        arguments += argv[i];
    }
    for (const auto& test: *_g_tests)
    {
        QStringList testArgs {"--test",test.name};
        auto status = QProcess::execute(argv[0],testArgs+arguments);
        if (status)
        {
            return(status);
        }
    }
    return 0;
}


void initialize(
)
{
    G_ASSERT(!_g_tests);
    _g_tests = new QList<TestObject> {
        {"Block",new Block::Abstract}
        ,{"BlockCppClass",new Block::Cpp::Class}
        ,{"BlockCppEnumeration",new Block::Cpp::Enumeration}
        ,{"BlockCppException",new Block::Cpp::ExceptionItem}
        ,{"BlockCppFunction",new Block::Cpp::Function}
        ,{"BlockCppNamespace",new Block::Cpp::Namespace}
        ,{"BlockCppProperty",new Block::Cpp::Property}
        ,{"BlockCppVariable",new Block::Cpp::Variable}
        ,{"BlockCppQtFunction",new Block::CppQt::Function}
        ,{"BlockCppQtProperty",new Block::CppQt::Property}
        ,{"FactoryLanguage",new Factory::Language}
        ,{"Language",new Language::Abstract}
        ,{"LanguageCpp",new Language::Cpp}
        ,{"LanguageCppQt",new Language::CppQt}
        ,{"ModelProject",new Model::Project}
    };
}
}
