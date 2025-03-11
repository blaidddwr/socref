#include <QtTest>
#include "CppBlock.h"
#include "CppBlockUnion.h"
#include "CppLanguage.h"
#include "CppParseUnionParser.h"
#include "Exception.h"
#include "../utility.h"
using namespace Cpp::Block;
using namespace Cpp::Parse;
using namespace Cpp;

class TestCppParseUnionParser: public QObject, private TestParse
{
    Q_OBJECT
    Language* _language;
private slots:
    void initTestCase();
    void children();
    void legacyParse1();
    void cleanupTestCase();
};

void TestCppParseUnionParser::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Language;
    _language->setParent(this);
}

void TestCppParseUnionParser::children()
{
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    UnionParser parser(qobject_cast<Namespace*>(root.get()),Cpp_Legacy);
    QCOMPARE(parser.size(),0);
}

void TestCppParseUnionParser::legacyParse1()
{
    using Status = AbstractParser::Status;
    auto lines = getLines("legacyParse1");
    auto testBody1 = lines.mid(2,2);
    auto testBody2 = lines.mid(8,2);
    std::unique_ptr<AbstractBlock> root(_language->createBlock(NamespaceIndex));
    auto test1 = qobject_cast<Union*>(_language->createBlock(UnionIndex));
    auto test2 = qobject_cast<Union*>(_language->createBlock(UnionIndex));
    test1->setName("Test1");
    test2->setName("Test2");
    root->append(test1);
    root->append(test2);
    try
    {
        UnionParser parser(root.get(),Cpp_Legacy);
        int where = 0;
        while (where < lines.size())
        {
            switch (where+1)
            {
            case 5:
            case 11:
                QCOMPARE(parser.parse(lines,where++),Status::DoneWithRead);
                break;
            case 6:
            case 12:
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
    QCOMPARE(test1->code().size(),1);
    QCOMPARE(test1->code().value(codeKey(BodyCodeKey)),testBody1);
    QCOMPARE(test2->code().size(),1);
    QCOMPARE(test2->code().value(codeKey(BodyCodeKey)),testBody2);
}

void TestCppParseUnionParser::cleanupTestCase()
{
    delete _language;
}

QTEST_MAIN(TestCppParseUnionParser)
#include "test.moc"
