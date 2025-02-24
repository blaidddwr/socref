#include <QtTest>
#include "CppBlock.h"
#include "CppBlockNamespace.h"
#include "CppLanguage.h"
#include "CppParseSourceParser.h"
using namespace Cpp::Block;
using namespace Cpp::Parse;
using namespace Cpp;

class TestCppParseSourceParser: public QObject
{
    Q_OBJECT
    Language* _language;
private slots:
    void initTestCase();
    void parseLegacy1();
    void parseLegacy2();
    void parseLegacy3();
    void parseLegacy4();
    void cleanupTestCase();
};

void TestCppParseSourceParser::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Language;
    _language->setParent(this);
}

void TestCppParseSourceParser::parseLegacy1()
{
    using Status = AbstractParser::Status;
    static const QStringList lines
        {
            "#include \"test.h\""
            ,""
            ,""
        };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    SourceParser parser(qobject_cast<Namespace*>(root.get()),Cpp_Legacy);
    int where = 0;
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseSourceParser::parseLegacy2()
{
    using Status = AbstractParser::Status;
    static const QStringList lines
        {
            "#include \"test.h\""
            ,"namespace Dummy {"
            ,""
            ,"}"
            ,""
        };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    SourceParser parser(qobject_cast<Namespace*>(root.get()),Cpp_Legacy);
    int where = 0;
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::DelegateToChildren);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseSourceParser::parseLegacy3()
{
    using Status = AbstractParser::Status;
    static const QStringList lines
        {
            "#include \"test.h\""
            ,"#include <one>"
            ,"#include \"two.h\""
            ,"#define THREE 0"
            ,""
            ,""
        };
    static const QStringList testPreProcess
        {
            "#include <one>"
            ,"#include \"two.h\""
            ,"#define THREE 0"
        };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    SourceParser parser(qobject_cast<Namespace*>(root.get()),Cpp_Legacy);
    int where = 0;
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QVERIFY(root->code().contains(codeKey(PreProcessSourceCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessSourceCodeKey)),testPreProcess);
}

void TestCppParseSourceParser::parseLegacy4()
{
    using Status = AbstractParser::Status;
    static const QStringList lines
        {
            "#include \"test.h\""
            ,"#include <one>"
            ,"#include \"two.h\""
            ,"#define THREE 0"
            ,"namespace Dummy {"
            ,"using header1;"
            ,"using ok = header2:ok;"
            ,""
            ,"}"
            ,""
        };
    static const QStringList testPreProcess
        {
            "#include <one>"
            ,"#include \"two.h\""
            ,"#define THREE 0"
        };
    static const QStringList testHeader
        {
            "using header1;"
            ,"using ok = header2:ok;"
        };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    SourceParser parser(qobject_cast<Namespace*>(root.get()),Cpp_Legacy);
    int where = 0;
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::DelegateToChildren);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
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
