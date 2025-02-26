#include <QtTest>
#include "CppBlock.h"
#include "CppBlockClass.h"
#include "CppLanguage.h"
#include "CppParseClassParser.h"
using namespace Cpp::Block;
using namespace Cpp::Parse;
using namespace Cpp;

class TestCppParseClassParser: public QObject
{
    Q_OBJECT
    Language* _language;
private slots:
    void initTestCase();
    void parse1Legacy();
    void parse2Legacy();
    void parse3Legacy();
    void parse4Legacy();
    void parse1Version1();
    void parse2Version1();
    void parse3Version1();
    void parse4Version1();
    void cleanupTestCase();
private:
    QStringList getLines(const QString& name);
};

void TestCppParseClassParser::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Language;
    _language->setParent(this);
}

void TestCppParseClassParser::parse1Legacy()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("parse1Legacy.cpp");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_Legacy);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseClassParser::parse2Legacy()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("parse2Legacy.cpp");
    auto testHeader = lines.mid(2,6);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_Legacy);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while (where < 8)
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QCOMPARE(root->code().value(codeKey(HeaderCodeKey)),testHeader);
}

void TestCppParseClassParser::parse3Legacy()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("parse3Legacy.cpp");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_Legacy);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while (where < 12)
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseClassParser::parse4Legacy()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("parse4Legacy.cpp");
    auto testHeader = lines.mid(2,3);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_Legacy);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while (where < 12)
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QCOMPARE(root->code().value(codeKey(HeaderCodeKey)),testHeader);
}

void TestCppParseClassParser::parse1Version1()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("parse1Legacy.cpp");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_1);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::Read);
    QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseClassParser::parse2Version1()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("parse2Legacy.cpp");
    auto testHeader = lines.mid(2,6);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_1);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while (where < 8)
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
    QCOMPARE(root->code().size(),1);
    QCOMPARE(root->code().value(codeKey(HeaderCodeKey)),testHeader);
}

void TestCppParseClassParser::parse3Version1()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("parse3Version1.cpp");
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_1);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while (where < 17)
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
    QCOMPARE(root->code().size(),0);
}

void TestCppParseClassParser::parse4Version1()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("parse4Version1.cpp");
    auto testHeader = lines.mid(2,2);
    auto testFooter = lines.mid(22,2);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(ClassIndex));
    auto cb = qobject_cast<Class*>(root.get());
    cb->setName("Test");
    ClassParser parser(qobject_cast<Class*>(root.get()),Cpp_1);
    QVERIFY(parser.children().isEmpty());
    int where = 0;
    while (where < 24)
    {
        QCOMPARE(parser.parse(lines,where++),Status::Read);
    }
    QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
    QCOMPARE(root->code().size(),2);
    QCOMPARE(root->code().value(codeKey(HeaderCodeKey)),testHeader);
    QCOMPARE(root->code().value(codeKey(FooterCodeKey)),testFooter);
}

void TestCppParseClassParser::cleanupTestCase()
{
    delete _language;
}

QStringList TestCppParseClassParser::getLines(const QString& name)
{
    QFile file(":/"+name);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw std::logic_error("missing test file");
    }
    QTextStream stream(&file);
    return stream.readAll().split("\n",Qt::KeepEmptyParts);
    return {};
}

QTEST_MAIN(TestCppParseClassParser)
#include "test.moc"
