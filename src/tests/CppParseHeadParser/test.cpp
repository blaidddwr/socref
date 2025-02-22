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
    void parse1Legacy();
    void parse2Legacy();
    void parse3Legacy();
    void parse4Legacy();
    void parse5Legacy();
    void parse1Version1();
    void parse2Version1();
    void parse3Version1();
    void parse4Version1();
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

void TestCppParseHeadParser::parse1Legacy()
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
    //QCOMPARE(_parser->children().size(),2);//TODO
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::parse2Legacy()
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
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    _parser->setVersion(Cpp_Legacy);
    _parser->setBlock(root.get());
    _parser->reset();
    int where = 0;
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    //QCOMPARE(_parser->children().size(),1);//TODO
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::parse3Legacy()
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
    //QCOMPARE(_parser->children().size(),2);//TODO
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::parse4Legacy()
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
    //QCOMPARE(_parser->children().size(),2);//TODO
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QVERIFY(root->code().contains(codeKey(PreProcessHeadCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessHeadCodeKey)),testPreProcess);
}

void TestCppParseHeadParser::parse5Legacy()
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
    //QCOMPARE(_parser->children().size(),2);//TODO
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QVERIFY(root->code().contains(codeKey(PreProcessHeadCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessHeadCodeKey)),testPreProcess);
}

void TestCppParseHeadParser::parse1Version1()
{
    using Status = AbstractParser::Status;
    static const QStringList lines
        {
            "#ifndef TEST_H"
            ,"#define TEST_H"
            ,""
            ,"/*@ EOS @*/"
            ,""
            ,"/* comments */"
            ,""
            ,"#endif"
            ,""
        };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    _parser->setVersion(Cpp_1);
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
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::parse2Version1()
{
    using Status = AbstractParser::Status;
    static const QStringList lines
        {
            "#ifndef TEST_H"
            ,"#define TEST_H"
            ,"namespace Dummy {"
            ,""
            ,"}"
            ,""
            ,"/*@ EOS @*/"
            ,""
            ,"/* comments */"
            ,""
            ,"#endif"
            ,""
        };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    _parser->setVersion(Cpp_1);
    _parser->setBlock(root.get());
    _parser->reset();
    int where = 0;
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    //QCOMPARE(_parser->children().size(),2);//TODO
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::parse3Version1()
{
    using Status = AbstractParser::Status;
    static const QStringList lines
        {
            "#ifndef TEST_H"
            ,"#define TEST_H"
            ,"#include <one>"
            ,"#include \"two.h\""
            ,"#define THREE 0"
            ,"/*@ header @*/"
            ,"using header1;"
            ,"using ok = header2::ok;"
            ,""
            ,"function Foo();"
            ,""
            ,"/*@ footer @*/"
            ,"class Simple {};"
            ,"class AnotherOne : public Simple { using Simple::Simple; }"
            ,"/*@ end @*/"
            ,""
            ,"/*@ EOS @*/"
            ,""
            ,"/* comments */"
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
    static const QStringList testHeader
        {
            "using header1;"
            ,"using ok = header2::ok;"
        };
    static const QStringList testFooter
        {
            "class Simple {};"
            ,"class AnotherOne : public Simple { using Simple::Simple; }"
        };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    _parser->setVersion(Cpp_1);
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
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    //QCOMPARE(_parser->children().size(),2);//TODO
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),3);
    QVERIFY(root->code().contains(codeKey(PreProcessHeadCodeKey)));
    QVERIFY(root->code().contains(codeKey(HeaderHeadCodeKey)));
    QVERIFY(root->code().contains(codeKey(FooterHeadCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessHeadCodeKey)),testPreProcess);
    QCOMPARE(root->code().value(codeKey(HeaderHeadCodeKey)),testHeader);
    QCOMPARE(root->code().value(codeKey(FooterHeadCodeKey)),testFooter);
}

void TestCppParseHeadParser::parse4Version1()
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
            ,"using header1;"
            ,"using ok = header2::ok;"
            ,""
            ,"function Foo();"
            ,""
            ,"/*@ footer @*/"
            ,"class Simple {};"
            ,"class AnotherOne : public Simple { using Simple::Simple; }"
            ,"}"
            ,""
            ,"/*@ EOS @*/"
            ,""
            ,"/* comments */"
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
    static const QStringList testHeader
        {
            "using header1;"
            ,"using ok = header2::ok;"
        };
    static const QStringList testFooter
        {
            "class Simple {};"
            ,"class AnotherOne : public Simple { using Simple::Simple; }"
        };
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    _parser->setVersion(Cpp_1);
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
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::DelegateToChildren);
    //QCOMPARE(_parser->children().size(),2);//TODO
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,where++),Status::Read);
    QCOMPARE(_parser->parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),3);
    QVERIFY(root->code().contains(codeKey(PreProcessHeadCodeKey)));
    QVERIFY(root->code().contains(codeKey(HeaderHeadCodeKey)));
    QVERIFY(root->code().contains(codeKey(FooterHeadCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessHeadCodeKey)),testPreProcess);
    QCOMPARE(root->code().value(codeKey(HeaderHeadCodeKey)),testHeader);
    QCOMPARE(root->code().value(codeKey(FooterHeadCodeKey)),testFooter);
}

void TestCppParseHeadParser::cleanupTestCase()
{
    delete _parser;
    delete _language;
}

QTEST_MAIN(TestCppParseHeadParser)
#include "test.moc"
