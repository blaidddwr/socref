#include <QtTest>
#include "CppBlock.h"
#include "CppBlockClass.h"
#include "CppLanguage.h"
#include "CppParseClassParser.h"
#include "../utility.h"
using namespace Cpp::Block;
using namespace Cpp::Parse;
using namespace Cpp;

class TestCppParseClassParser: public QObject, private TestParse
{
    Q_OBJECT
    Language* _language;
private slots:
    void initTestCase();
    void legacyParse1();
    void legacyParse2();
    void legacyParse3();
    void legacyParse4();
    void version1Parse1();
    void version1Parse2();
    void version1Parse3();
    void version1Parse4();
    void cleanupTestCase();
};

void TestCppParseClassParser::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Language;
    _language->setParent(this);
}

void TestCppParseClassParser::legacyParse1()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse1");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    QVERIFY(cb);
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_Legacy);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while ((where+2) < lines.size())
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseClassParser::legacyParse2()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse2");
    auto testHeader = lines.mid(2,6);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    QVERIFY(cb);
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_Legacy);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while ((where+2) < lines.size())
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QCOMPARE(root->code().value(codeKey(HeaderCodeKey)),testHeader);
}

void TestCppParseClassParser::legacyParse3()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse3");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    QVERIFY(cb);
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_Legacy);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while ((where+2) < lines.size())
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseClassParser::legacyParse4()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse4");
    auto testHeader = lines.mid(2,3);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    QVERIFY(cb);
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_Legacy);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while ((where+2) < lines.size())
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QCOMPARE(root->code().value(codeKey(HeaderCodeKey)),testHeader);
}

void TestCppParseClassParser::version1Parse1()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse1");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    QVERIFY(cb);
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_1);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while ((where+2) < lines.size())
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseClassParser::version1Parse2()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse2");
    auto testHeader = lines.mid(2,6);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    QVERIFY(cb);
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_1);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while ((where+2) < lines.size())
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QCOMPARE(root->code().value(codeKey(HeaderCodeKey)),testHeader);
}

void TestCppParseClassParser::version1Parse3()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("version1Parse3");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    QVERIFY(cb);
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_1);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while ((where+2) < lines.size())
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseClassParser::version1Parse4()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("version1Parse4");
    auto testHeader = lines.mid(2,2);
    auto testFooter = lines.mid(22,2);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    QVERIFY(cb);
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_1);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while ((where+2) < lines.size())
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where),Status::DoneWithRead);
    QCOMPARE(root->code().size(),2);
    QCOMPARE(root->code().value(codeKey(HeaderCodeKey)),testHeader);
    QCOMPARE(root->code().value(codeKey(FooterCodeKey)),testFooter);
}

void TestCppParseClassParser::cleanupTestCase()
{
    delete _language;
}

QTEST_MAIN(TestCppParseClassParser)
#include "test.moc"
