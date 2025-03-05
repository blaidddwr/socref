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
    void legacyParse5();
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
            switch (where+1)
            {
            case 3:
            case 4:
            case 5:
                QCOMPARE(parser.parse(lines,where++),Status::Read);
                break;
            case 6:
                QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
                break;
            default:
                QCOMPARE(parser.parse(lines,where++),Status::DoneWithoutRead);
                break;
            }
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

void TestCppParseFunctionParser::legacyParse5()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse5");
    auto testBody1 = lines.mid(2,2);
    auto testBody2 = lines.mid(11,2);
    auto testBody3 = lines.mid(17,2);
    auto testBody4 = lines.mid(23,2);
    auto testBody5 = lines.mid(31,2);
    auto testBody6 = lines.mid(39,2);
    auto testBody7 = lines.mid(45,2);
    auto testBody8 = lines.mid(51,2);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    auto class1 = qobject_cast<Class*>(_language->createBlock(ClassIndex));
    auto function1 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function2 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function3 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function4 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function5 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function6 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function7 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto function8 = qobject_cast<Function*>(_language->createBlock(FunctionIndex));
    auto arg1 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg2 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg3 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg4 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg5 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg6 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg7 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg8 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg9 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg10 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    auto arg11 = qobject_cast<Variable*>(_language->createBlock(VariableIndex));
    arg1->setName("arg1");
    arg1->setType("const class1&");
    arg2->setName("arg2");
    arg2->setType("const class1&");
    arg3->setName("arg3");
    arg3->setType("const class1&");
    arg4->setName("arg4");
    arg4->setType("const class1&");
    arg5->setName("arg5");
    arg5->setType("size_t");
    arg6->setName("arg6");
    arg6->setType("void*");
    arg7->setName("arg7");
    arg7->setType("const class1&");
    arg8->setName("arg8");
    arg8->setType("const class1&");
    arg9->setName("arg9");
    arg9->setType("size_t");
    arg10->setName("arg10");
    arg10->setType("int");
    arg11->setName("arg11");
    arg11->setType("int");
    function1->append(arg1);
    function1->append(arg2);
    function2->append(arg3);
    function2->append(arg4);
    function3->append(arg5);
    function4->append(arg6);
    function5->append(arg7);
    function6->append(arg8);
    function7->append(arg9);
    function8->append(arg10);
    function8->append(arg11);
    class1->setName("class1");
    class1->append(function3);
    class1->append(function4);
    class1->append(function5);
    class1->append(function6);
    class1->append(function7);
    class1->append(function8);
    root->append(class1);
    root->append(function1);
    root->append(function2);
    function1->set("+","class1",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    function2->set("-","class1",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    function3->set(" new[]","void*",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    function4->set(" delete","void",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    function5->set("+=","class1&",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    function6->set("*","class1&",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    function7->set("<<","class1",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    function8->set("()","int",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    try
    {
        FunctionParser parser(root.get(),Cpp_Legacy);
        int where = 0;
        while (where < lines.size())
        {
            switch (where+1)
            {
            case 5:
            case 14:
            case 20:
            case 26:
            case 34:
            case 42:
            case 48:
            case 54:
                QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
                break;
            case 6:
            case 15:
            case 21:
            case 27:
            case 35:
            case 43:
            case 49:
            case 55:
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
    QCOMPARE(function4->code().size(),1);
    QCOMPARE(function4->code().value(codeKey(BodyCodeKey)),testBody4);
    QCOMPARE(function5->code().size(),1);
    QCOMPARE(function5->code().value(codeKey(BodyCodeKey)),testBody5);
    QCOMPARE(function6->code().size(),1);
    QCOMPARE(function6->code().value(codeKey(BodyCodeKey)),testBody6);
    QCOMPARE(function7->code().size(),1);
    QCOMPARE(function7->code().value(codeKey(BodyCodeKey)),testBody7);
    QCOMPARE(function8->code().size(),1);
    QCOMPARE(function8->code().value(codeKey(BodyCodeKey)),testBody8);
}

void TestCppParseFunctionParser::cleanupTestCase()
{
    delete _language;
}

QTEST_MAIN(TestCppParseFunctionParser)
#include "test.moc"
