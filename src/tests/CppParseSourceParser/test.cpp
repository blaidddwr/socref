#include <QtTest>
#include "CppBlock.h"
#include "CppBlockNamespace.h"
#include "CppLanguage.h"
#include "CppParseFunctionParser.h"
#include "CppParseSourceParser.h"
#include "Exception.h"
#include "../utility.h"
using namespace Cpp::Block;
using namespace Cpp::Parse;
using namespace Cpp;

class TestCppParseSourceParser: public QObject, private TestParse
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

void TestCppParseSourceParser::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Language;
    _language->setParent(this);
}

void TestCppParseSourceParser::children()
{
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    SourceParser parser(root.get(),Cpp_Legacy);
    QCOMPARE(parser.size(),1);
    QVERIFY(qobject_cast<FunctionParser*>(parser.get(0)));
}

void TestCppParseSourceParser::legacyParse1()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse1");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    try
    {
        SourceParser parser(root.get(),Cpp_Legacy);
        int where = 0;
        while (where < lines.size())
        {
            QCOMPARE(parser.parse(lines,where++),Status::Read);
        }
        QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    }
    catch (Exception::LogicalParse& e)
    {
        qDebug() << tr("Logical Parse Exception: %1").arg(e.message());
    }
    QCOMPARE(root->code().size(),0);
}

void TestCppParseSourceParser::legacyParse2()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse2");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    try
    {
        SourceParser parser(root.get(),Cpp_Legacy);
        int where = 0;
        while (where < lines.size())
        {
            switch (where+1)
            {
            case 4:
                QCOMPARE(parser.parse(lines,where++),Status::DelegateToChildren);
                break;
            default:
                QCOMPARE(parser.parse(lines,where++),Status::Read);
                break;
            }
        }
        QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    }
    catch (Exception::LogicalParse& e)
    {
        qDebug() << tr("Logical Parse Exception: %1").arg(e.message());
    }
    QCOMPARE(root->code().size(),0);
}

void TestCppParseSourceParser::legacyParse3()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse3");
    auto testPreProcess = lines.mid(1,3);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    try
    {
        SourceParser parser(root.get(),Cpp_Legacy);
        int where = 0;
        while (where < lines.size())
        {
            switch (where+1)
            {
            case 6:
            case 7:
            case 8:
            case 9:
                QCOMPARE(parser.parse(lines,where++),Status::DelegateToChildren);
                break;
            default:
                QCOMPARE(parser.parse(lines,where++),Status::Read);
                break;
            }
        }
        QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    }
    catch (Exception::LogicalParse& e)
    {
        qDebug() << tr("Logical Parse Exception: %1").arg(e.message());
    }
    QCOMPARE(root->code().size(),1);
    QVERIFY(root->code().contains(codeKey(PreProcessSourceCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessSourceCodeKey)),testPreProcess);
}

void TestCppParseSourceParser::legacyParse4()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse4");
    auto testPreProcess = lines.mid(1,3);
    auto testHeader = lines.mid(5,2);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    try
    {
        SourceParser parser(root.get(),Cpp_Legacy);
        int where = 0;
        while (where < lines.size())
        {
            switch (where+1)
            {
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
                QCOMPARE(parser.parse(lines,where++),Status::DelegateToChildren);
                break;
            default:
                QCOMPARE(parser.parse(lines,where++),Status::Read);
                break;
            }
        }
        QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    }
    catch (Exception::LogicalParse& e)
    {
        qDebug() << tr("Logical Parse Exception: %1").arg(e.message());
    }
    QCOMPARE(root->code().size(),2);
    QVERIFY(root->code().contains(codeKey(PreProcessSourceCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessSourceCodeKey)),testPreProcess);
    QVERIFY(root->code().contains(codeKey(HeaderSourceCodeKey)));
    QCOMPARE(root->code().value(codeKey(HeaderSourceCodeKey)),testHeader);
}

void TestCppParseSourceParser::cleanupTestCase()
{
    delete _language;
}

QTEST_MAIN(TestCppParseSourceParser)
#include "test.moc"
