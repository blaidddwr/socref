#include <QtTest>
#include "CppBlock.h"
#include "CppBlockClass.h"
#include "CppBlockNamespace.h"
#include "CppLanguage.h"
#include "CppParseClassParser.h"
#include "CppParseFunctionParser.h"
#include "CppParseHeadParser.h"
#include "../utility.h"
using namespace Cpp::Block;
using namespace Cpp::Parse;
using namespace Cpp;

class TestCppParseHeadParser: public QObject, private TestParse
{
    Q_OBJECT
    Language* _language;
private slots:
    void initTestCase();
    void children1();
    void children2();
    void legacyParse1();
    void legacyParse2();
    void legacyParse3();
    void legacyParse4();
    void legacyParse5();
    void version1Parse1();
    void version1Parse2();
    void version1Parse3();
    void version1Parse4();
    void cleanupTestCase();
};

void TestCppParseHeadParser::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Language;
    _language->setParent(this);
}

void TestCppParseHeadParser::children1()
{
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    HeadParser parser(qobject_cast<Class*>(root.get()),Cpp_Legacy);
    QCOMPARE(parser.children().size(),2);
    QVERIFY(qobject_cast<ClassParser*>(parser.children().at(0)));
    QVERIFY(qobject_cast<FunctionParser*>(parser.children().at(1)));
}

void TestCppParseHeadParser::children2()
{
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    HeadParser parser(qobject_cast<Namespace*>(root.get()),Cpp_Legacy);
    QCOMPARE(parser.children().size(),2);
    QVERIFY(qobject_cast<ClassParser*>(parser.children().at(0)));
    QVERIFY(qobject_cast<FunctionParser*>(parser.children().at(1)));
}

void TestCppParseHeadParser::legacyParse1()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse1");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    HeadParser parser(qobject_cast<Namespace*>(root.get()),Cpp_Legacy);
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
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::legacyParse2()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse1");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    HeadParser parser(qobject_cast<Class*>(root.get()),Cpp_Legacy);
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
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::legacyParse3()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse3");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    HeadParser parser(qobject_cast<Namespace*>(root.get()),Cpp_Legacy);
    int where = 0;
    while (where < lines.size())
    {
        switch (where+1)
        {
        case 5:
        case 6:
            QCOMPARE(parser.parse(lines,where++),Status::DelegateToChildren);
            break;
        default:
            QCOMPARE(parser.parse(lines,where++),Status::Read);
            break;
        }
    }
    QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::legacyParse4()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse4");
    auto testPreProcess = lines.mid(2,3);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    HeadParser parser(qobject_cast<Namespace*>(root.get()),Cpp_Legacy);
    int where = 0;
    while (where < lines.size())
    {
        switch (where+1)
        {
        case 7:
            QCOMPARE(parser.parse(lines,where++),Status::DelegateToChildren);
            break;
        default:
            QCOMPARE(parser.parse(lines,where++),Status::Read);
            break;
        }
    }
    QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QVERIFY(root->code().contains(codeKey(PreProcessHeadCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessHeadCodeKey)),testPreProcess);
}

void TestCppParseHeadParser::legacyParse5()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse5");
    auto testPreProcess = lines.mid(2,3);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    HeadParser parser(qobject_cast<Namespace*>(root.get()),Cpp_Legacy);
    int where = 0;
    while (where < lines.size())
    {
        switch (where+1)
        {
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
    QCOMPARE(root->code().size(),1);
    QVERIFY(root->code().contains(codeKey(PreProcessHeadCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessHeadCodeKey)),testPreProcess);
}

void TestCppParseHeadParser::version1Parse1()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("version1Parse1");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    HeadParser parser(qobject_cast<Namespace*>(root.get()),Cpp_1);
    int where = 0;
    while (where < lines.size())
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::version1Parse2()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("version1Parse2");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    HeadParser parser(qobject_cast<Namespace*>(root.get()),Cpp_1);
    int where = 0;
    while (where < lines.size())
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseHeadParser::version1Parse3()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("version1Parse3");
    auto testPreProcess = lines.mid(2,5);
    auto testFooter = lines.mid(14,2);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    HeadParser parser(qobject_cast<Namespace*>(root.get()),Cpp_1);
    int where = 0;
    while (where < lines.size())
    {
        switch (where+1)
        {
        case 9:
        case 10:
        case 11:
        case 13:
            QCOMPARE(parser.parse(lines,where++),Status::DelegateToChildren);
            break;
        default:
            QCOMPARE(parser.parse(lines,where++),Status::Read);
            break;
        }
    }
    QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
    QCOMPARE(root->code().size(),2);
    QVERIFY(root->code().contains(codeKey(PreProcessHeadCodeKey)));
    QVERIFY(root->code().contains(codeKey(FooterHeadCodeKey)));
    QCOMPARE(root->code().value(codeKey(PreProcessHeadCodeKey)),testPreProcess);
    QCOMPARE(root->code().value(codeKey(FooterHeadCodeKey)),testFooter);
}

void TestCppParseHeadParser::version1Parse4()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("version1Parse4");
    auto testPreProcess = lines.mid(2,3);
    auto testHeader = lines.mid(6,2);
    auto testFooter = lines.mid(15,2);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    HeadParser parser(qobject_cast<Namespace*>(root.get()),Cpp_1);
    int where = 0;
    while (where < lines.size())
    {
        switch (where+1)
        {
        case 10:
        case 11:
        case 12:
        case 14:
            QCOMPARE(parser.parse(lines,where++),Status::DelegateToChildren);
            break;
        default:
            QCOMPARE(parser.parse(lines,where++),Status::Read);
            break;
        }
    }
    QCOMPARE(parser.parse(lines,AbstractParser::EOL),Status::DoneWithRead);
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
    delete _language;
}

QTEST_MAIN(TestCppParseHeadParser)
#include "test.moc"
