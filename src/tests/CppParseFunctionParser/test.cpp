#include <QtTest>
#include "CppBlock.h"
#include "CppBlockClass.h"
#include "CppBlockFunction.h"
#include "CppBlockVariable.h"
#include "CppLanguage.h"
#include "CppParseFunctionParser.h"
#include "Exception.h"
#include "../utility.h"
using namespace Cpp::Block;
using namespace Cpp::Parse;
using namespace Cpp;

class TestCppParseFunctionParser: public QObject, private TestParse
{
    Q_OBJECT
    Language* _language;
private slots:
    void initTestCase();
    void children();
    void legacyParse1();
    void legacyParse2();
    void legacyParse3();
    void legacyParse4();
    void cleanupTestCase();
};

void TestCppParseFunctionParser::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Language;
    _language->setParent(this);
}

void TestCppParseFunctionParser::children()
{
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    FunctionParser parser(root.get(),Cpp_Legacy);
    QVERIFY(parser.children().isEmpty());
}

void TestCppParseFunctionParser::legacyParse1()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse1");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    try
    {
        FunctionParser parser(root.get(),Cpp_Legacy);
        int where = 0;
        while (where < lines.size())
        {
            QCOMPARE(parser.parse(lines,where++),Status::DoneWithoutRead);
        }
    }
    catch (Exception::LogicalParse& e)
    {
        qDebug() << tr("Logical Parse Exception: %1").arg(e.message());
    }
    QCOMPARE(root->code().size(),0);
}

void TestCppParseFunctionParser::legacyParse2()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse2");
    auto testBody1 = lines.mid(2,2);
    auto testBody2 = lines.mid(8,2);
    auto testBody3 = lines.mid(17,2);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    auto function1 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function2 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function3 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto arg1 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg2 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg3 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg4 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    arg1->setName("arg1");
    arg1->setType("int");
    arg2->setName("arg2");
    arg2->setType("int");
    arg3->setName("arg3");
    arg3->setType("int");
    arg4->setName("arg4");
    arg4->setType("const QList<int>&");
    function1->set("function1","void",RegularFunctionType,PublicAccess,NoFunctionAssignment,0);
    function2->set("function2","void",RegularFunctionType,PublicAccess,NoFunctionAssignment,0);
    function2->append(arg1);
    function2->append(arg2);
    function3->set("function3","void",RegularFunctionType,PublicAccess,NoFunctionAssignment,0);
    function3->append(arg3);
    function3->append(arg4);
    root->append(function1);
    root->append(function2);
    root->append(function3);
    try
    {
        FunctionParser parser(root.get(),Cpp_Legacy);
        int where = 0;
        while (where < lines.size())
        {
            switch (where+1)
            {
            case 5:
            case 11:
            case 20:
                QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
                break;
            case 6:
            case 12:
            case 21:
                QCOMPARE(parser.parse(lines,where++),Status::DoneWithoutRead);
                break;
            default:
                QCOMPARE(parser.parse(lines,where++),Status::Read);
                break;
            }
        }
    }
    catch (Exception::LogicalParse& e)
    {
        qDebug() << tr("Logical Parse Exception: %1").arg(e.message());
    }
    QCOMPARE(root->code().size(),0);
    QCOMPARE(function1->code().size(),1);
    QCOMPARE(function1->code().value(codeKey(BodyCodeKey)),testBody1);
    QCOMPARE(function2->code().size(),1);
    QCOMPARE(function2->code().value(codeKey(BodyCodeKey)),testBody2);
    QCOMPARE(function3->code().size(),1);
    QCOMPARE(function3->code().value(codeKey(BodyCodeKey)),testBody3);
}

void TestCppParseFunctionParser::legacyParse3()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse3");
    auto testBody1 = lines.mid(2,2);
    auto testBody2 = lines.mid(8,2);
    auto testBody3 = lines.mid(17,2);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    auto class1 = qobject_cast<Class*>(_language->createBlock(ClassIndex));
    auto function1 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function2 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function3 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto arg1 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg2 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg3 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg4 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    arg1->setName("arg1");
    arg1->setType("int");
    arg2->setName("arg2");
    arg2->setType("int");
    arg3->setName("arg3");
    arg3->setType("int");
    arg4->setName("arg4");
    arg4->setType("const QList<int>&");
    function1->set("function1","void",RegularFunctionType,PublicAccess,NoFunctionAssignment,0);
    function2->set("function2","void",RegularFunctionType,PublicAccess,NoFunctionAssignment,0);
    function2->append(arg1);
    function2->append(arg2);
    function3->set("function3","void",RegularFunctionType,PublicAccess,NoFunctionAssignment,0);
    function3->append(arg3);
    function3->append(arg4);
    class1->setName("class1");
    class1->append(function1);
    class1->append(function2);
    class1->append(function3);
    root->append(class1);
    try
    {
        FunctionParser parser(root.get(),Cpp_Legacy);
        int where = 0;
        while (where < lines.size())
        {
            switch (where+1)
            {
            case 5:
            case 11:
            case 20:
                QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
                break;
            case 6:
            case 12:
            case 21:
                QCOMPARE(parser.parse(lines,where++),Status::DoneWithoutRead);
                break;
            default:
                QCOMPARE(parser.parse(lines,where++),Status::Read);
                break;
            }
        }
    }
    catch (Exception::LogicalParse& e)
    {
        qDebug() << tr("Logical Parse Exception: %1").arg(e.message());
    }
    QCOMPARE(root->code().size(),0);
    QCOMPARE(class1->code().size(),0);
    QCOMPARE(function1->code().size(),1);
    QCOMPARE(function1->code().value(codeKey(BodyCodeKey)),testBody1);
    QCOMPARE(function2->code().size(),1);
    QCOMPARE(function2->code().value(codeKey(BodyCodeKey)),testBody2);
    QCOMPARE(function3->code().size(),1);
    QCOMPARE(function3->code().value(codeKey(BodyCodeKey)),testBody3);
}

void TestCppParseFunctionParser::legacyParse4()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse4");
    auto testHeader1 = lines.mid(1,2);
    auto testBody1 = lines.mid(4,2);
    auto testHeader2 = lines.mid(9,2);
    auto testBody2 = lines.mid(12,2);
    auto testHeader3 = lines.mid(20,2);
    auto testBody3 = lines.mid(23,2);
    auto testBody4 = lines.mid(29,2);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto class1 = qobject_cast<Class*>(root.get());
    auto function1 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function2 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function3 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function4 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto arg1 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg2 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg3 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg4 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    arg1->setName("arg1");
    arg1->setType("int");
    arg2->setName("arg2");
    arg2->setType("int");
    arg3->setName("arg3");
    arg3->setType("const QString&");
    arg4->setName("arg4");
    arg4->setType("int");
    function2->append(arg1);
    function2->append(arg2);
    function3->append(arg3);
    function3->append(arg4);
    class1->setName("class1");
    class1->append(function1);
    class1->append(function2);
    class1->append(function3);
    class1->append(function4);
    function1->set("","",ConstructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    function2->set("","",ConstructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    function3->set("","",ConstructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    function4->set("","",DestructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    try
    {
        FunctionParser parser(root.get(),Cpp_Legacy);
        int where = 0;
        while (where < lines.size())
        {
            switch (where+1)
            {
            case 7:
            case 15:
            case 26:
            case 32:
                QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
                break;
            case 8:
            case 16:
            case 27:
            case 33:
                QCOMPARE(parser.parse(lines,where++),Status::DoneWithoutRead);
                break;
            default:
                QCOMPARE(parser.parse(lines,where++),Status::Read);
                break;
            }
        }
    }
    catch (Exception::LogicalParse& e)
    {
        qDebug() << tr("Logical Parse Exception: %1").arg(e.message());
    }
    QCOMPARE(root->code().size(),0);
    QCOMPARE(class1->code().size(),0);
    QCOMPARE(function1->code().size(),2);
    QCOMPARE(function1->code().value(codeKey(HeaderCodeKey)),testHeader1);
    QCOMPARE(function1->code().value(codeKey(BodyCodeKey)),testBody1);
    QCOMPARE(function2->code().size(),2);
    QCOMPARE(function2->code().value(codeKey(HeaderCodeKey)),testHeader2);
    QCOMPARE(function2->code().value(codeKey(BodyCodeKey)),testBody2);
    QCOMPARE(function3->code().size(),2);
    QCOMPARE(function3->code().value(codeKey(HeaderCodeKey)),testHeader3);
    QCOMPARE(function3->code().value(codeKey(BodyCodeKey)),testBody3);
    QCOMPARE(function4->code().size(),1);
    QCOMPARE(function4->code().value(codeKey(BodyCodeKey)),testBody4);
}

void TestCppParseFunctionParser::cleanupTestCase()
{
    delete _language;
}

QTEST_MAIN(TestCppParseFunctionParser)
#include "test.moc"
