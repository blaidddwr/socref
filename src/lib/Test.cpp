#include "Test.h"
#include <QtTest>
#include "TestBlockAbstract.h"
#include "TestBlockCppClass.h"
#include "TestBlockCppEnumeration.h"
#include "TestBlockCppExceptionItem.h"
#include "TestBlockCppFunction.h"
#include "TestBlockCppNamespace.h"
#include "TestBlockCppProperty.h"
#include "TestBlockCppQtFunction.h"
#include "TestBlockCppVariable.h"
#include "TestFactoryLanguage.h"
#include "TestLanguageAbstract.h"
#include "TestLanguageCpp.h"
#include "TestLanguageCppQt.h"
#include "TestModelProject.h"
namespace Test {
QStringList _g_qtUnitTestArgs;
const QList<TestObject>* _g_tests {nullptr};


bool areIconsEqual(
    const QIcon& icon0
    ,const QIcon& icon1
)
{
    QByteArray data0;
    QByteArray data1;
    QDataStream in0(&data0,QIODevice::WriteOnly);
    QDataStream in1(&data1,QIODevice::WriteOnly);
    in0 << icon0;
    in1 << icon1;
    return data0 == data1;
}


int execute(
    const QString& name
)
{
    if (name.isEmpty())
    {
        listTests();
        return 0;
    }
    for (const auto& test: testObjects())
    {
        if (test.name == name)
        {
            return QTest::qExec(
                test.ptr
                ,QList({QCoreApplication::arguments().at(0)})+_g_qtUnitTestArgs
            );
        }
    }
    QTextStream out(stdout);
    out << QApplication::translate("main","%1: Unknown unit test '%2'.\n")
        .arg(QApplication::applicationName(),name);
    listTests();
    return 1;
}


int executeAll(
)
{
    for (const auto& test: testObjects())
    {
        auto status = QProcess::execute(
            QCoreApplication::arguments().at(0)
            ,QStringList({"--test",test.name,":"})+_g_qtUnitTestArgs
        );
        if (status)
        {
            return(status);
        }
    }
    return 0;
}


void extractArguments(
    int& argc
    ,char** argv
)
{
    int total = argc;
    for (int i = 1;i < total;i++)
    {
        if (!strcmp(argv[i],":"))
        {
            argc = i;
            break;
        }
    }
    for (int i = argc+1;i < total;i++)
    {
        _g_qtUnitTestArgs.append(argv[i]);
    }
}


void listTests(
)
{
    QTextStream out(stdout);
    out << QApplication::translate("main","Available unit tests:\n");
    for (const auto& test: testObjects())
    {
        out << test.name << "\n";
    }
}


const QList<TestObject>& testObjects(
)
{
    if (!_g_tests)
    {
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
            ,{"FactoryLanguage",new Factory::Language}
            ,{"Language",new Language::Abstract}
            ,{"LanguageCpp",new Language::Cpp}
            ,{"LanguageCppQt",new Language::CppQt}
            ,{"ModelProject",new Model::Project}
        };
    }
    return *_g_tests;
}
}
