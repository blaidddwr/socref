#include <QtTest>
#include "CppBlock.h"
#include "CppBlockNamespace.h"
#include "CppLanguage.h"
#include "CppParseHeadParser.h"
using namespace Cpp::Block;
using namespace Cpp::Parse;
using namespace Cpp;

class TestCppParseHeadParser: public QObject
{
    Q_OBJECT
    HeadParser* _parser;
    Language* _language;
private slots:
    void initTestCase();
    void parseLegacy1();
    void parseLegacy2();
    void parseLegacy3();
    void parseLegacy4();
    void cleanupTestCase();
};

void TestCppParseHeadParser::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Language;
    _language->setParent(this);
    _parser = new HeadParser;
    _parser->setParent(this);
}

void TestCppParseHeadParser::parseLegacy1()
{
    using Status = AbstractParser::Status;
    static const QStringList lines
        {
            "#ifndef TEST_H"
            ,"#define TEST_H"
            ,""
            ,"#endif"
            ,""
        };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    _parser->setVersion(Cpp_Legacy);
    _parser->setBlock(root.get());
    int where = 0;
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::parseLegacy2()
{
    using Status = AbstractParser::Status;
    static const QStringList lines
        {
            "#ifndef TEST_H"
            ,"#define TEST_H"
            ,"namespace Dummy {"
            ,""
            ,"}"
            ,"#endif"
            ,""
        };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    _parser->setVersion(Cpp_Legacy);
    _parser->setBlock(root.get());
    _parser->reset();
    int where = 0;
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::parseLegacy3()
{
    using Status = AbstractParser::Status;
    static const QStringList lines
        {
            "#ifndef TEST_H"
            ,"#define TEST_H"
            ,"#include <one>"
            ,"#include \"two.h\""
            ,"#define THREE 0"
            ,""
            ,"#endif"
            ,""
        };
    static const QStringList testPreProcess
        {
            "#include <one>"
            ,"#include \"two.h\""
            ,"#define THREE 0"
        };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    _parser->setVersion(Cpp_Legacy);
    _parser->setBlock(root.get());
    _parser->reset();
    int where = 0;
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QVERIFY(root->code().contains(codeKey(PreProcessHeadCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessHeadCodeKey)),testPreProcess);
}

void TestCppParseHeadParser::parseLegacy4()
{
    using Status = AbstractParser::Status;
    static const QStringList lines
    {
        "#ifndef TEST_H"
        ,"#define TEST_H"
        ,"#include <one>"
        ,"#include \"two.h\""
        ,"#define THREE 0"
        ,"namespace Dummy {"
        ,""
        ,"}"
        ,"#endif"
        ,""
    };
    static const QStringList testPreProcess
    {
        "#include <one>"
        ,"#include \"two.h\""
        ,"#define THREE 0"
    };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    _parser->setVersion(Cpp_Legacy);
    _parser->setBlock(root.get());
    _parser->reset();
    int where = 0;
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QVERIFY(root->code().contains(codeKey(PreProcessHeadCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessHeadCodeKey)),testPreProcess);
}

void TestCppParseHeadParser::cleanupTestCase()
{
    delete _parser;
    delete _language;
}

QTEST_MAIN(TestCppParseHeadParser)
#include "test.moc"
