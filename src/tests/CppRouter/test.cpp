#include <QtTest>
#include "CppBlock.h"
#include "CppBlockClass.h"
#include "CppBlockFunction.h"
#include "CppBlockNamespace.h"
#include "CppLanguage.h"
#include "CppParse.h"
#include "CppRouter.h"
#include "Exception.h"
using namespace Cpp::Block;
using namespace Cpp::Parse;
using namespace Cpp;

class TestCppRouter: public QObject
{
    Q_OBJECT
    Language* _language;
    Router* _router;
private slots:
    void initTestCase();
    void routes1();
    void routes2();
    void routes3();
    void routes4();
    void cleanupTestCase();
private:
    template<class T, int index> T* appendBlock(AbstractBlock* block)
    {
        T* ret = qobject_cast<T*>(_language->createBlock(index));
        Q_ASSERT(ret);
        block->append(ret);
        return ret;
    }
};

void TestCppRouter::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    _language = new Language;
    _language->setParent(this);
    _router = new Router(this);
}

void TestCppRouter::routes1()
{
    std::unique_ptr<Namespace> root(qobject_cast<Namespace*>(_language->createRootBlock()));
    QVERIFY(root);
    auto cb = appendBlock<Class,ClassIndex>(root.get());
    auto fb = appendBlock<Function,FunctionIndex>(cb);
    fb->setTemplates({"Class A"});
    cb->setName("Test");
    try
    {
        auto routes = _router->routes(root.get());
        std::sort(
            routes.begin()
            ,routes.end()
            ,[] (const Shape::Route& a, const Shape::Route& b)
            {
                return a.path <= b.path;
            }
            );
        QCOMPARE(routes.size(),2);
        QCOMPARE(routes.at(0).path,"Global.h");
        QCOMPARE(routes.at(0).block,root.get());
        QCOMPARE(routes.at(0).parseIndex,HeadParserIndex);
        QCOMPARE(routes.at(0).buildIndex,-1);//TODO
        QCOMPARE(routes.at(1).path,"Test.h");
        QCOMPARE(routes.at(1).block,cb);
        QCOMPARE(routes.at(1).parseIndex,HeadParserIndex);
        QCOMPARE(routes.at(1).buildIndex,-1);//TODO
    }
    catch (Exception::LogicalRoute& e)
    {
        qDebug() << tr("Logical Route Exception: %1").arg(e.message());
        QVERIFY(false);
    }
}

void TestCppRouter::routes2()
{
    std::unique_ptr<Namespace> root(qobject_cast<Namespace*>(_language->createRootBlock()));
    QVERIFY(root);
    auto namespace1 = appendBlock<Namespace,NamespaceIndex>(root.get());
    auto class1 = appendBlock<Class,ClassIndex>(namespace1);
    appendBlock<Function,FunctionIndex>(class1);
    namespace1->setName("Namespace1");
    class1->setName("Class1");
    try
    {
        auto routes = _router->routes(root.get());
        std::sort(
            routes.begin()
            ,routes.end()
            ,[] (const Shape::Route& a, const Shape::Route& b)
            {
                return a.path <= b.path;
            }
            );
        QCOMPARE(routes.size(),3);
        QCOMPARE(routes.at(0).path,"Namespace1.h");
        QCOMPARE(routes.at(0).block,namespace1);
        QCOMPARE(routes.at(0).parseIndex,HeadParserIndex);
        QCOMPARE(routes.at(0).buildIndex,-1);//TODO
        QCOMPARE(routes.at(1).path,"Namespace1Class1.cpp");
        QCOMPARE(routes.at(1).block,class1);
        QCOMPARE(routes.at(1).parseIndex,SourceParserIndex);
        QCOMPARE(routes.at(1).buildIndex,-1);//TODO
        QCOMPARE(routes.at(2).path,"Namespace1Class1.h");
        QCOMPARE(routes.at(2).block,class1);
        QCOMPARE(routes.at(2).parseIndex,HeadParserIndex);
        QCOMPARE(routes.at(2).buildIndex,-1);//TODO
    }
    catch (Exception::LogicalRoute& e)
    {
        qDebug() << tr("Logical Route Exception: %1").arg(e.message());
        QVERIFY(false);
    }
}

void TestCppRouter::routes3()
{
    std::unique_ptr<Namespace> root(qobject_cast<Namespace*>(_language->createRootBlock()));
    QVERIFY(root);
    auto namespace1 = appendBlock<Namespace,NamespaceIndex>(root.get());
    auto namespace2 = appendBlock<Namespace,NamespaceIndex>(namespace1);
    appendBlock<Function,FunctionIndex>(root.get());
    appendBlock<Function,FunctionIndex>(namespace2);
    namespace1->setName("Namespace1");
    namespace2->setName("Namespace2");
    try
    {
        auto routes = _router->routes(root.get());
        std::sort(
            routes.begin()
            ,routes.end()
            ,[] (const Shape::Route& a, const Shape::Route& b)
            {
                return a.path <= b.path;
            }
            );
        QCOMPARE(routes.size(),4);
        QCOMPARE(routes.at(0).path,"Global.cpp");
        QCOMPARE(routes.at(0).block,root.get());
        QCOMPARE(routes.at(0).parseIndex,SourceParserIndex);
        QCOMPARE(routes.at(0).buildIndex,-1);//TODO
        QCOMPARE(routes.at(1).path,"Global.h");
        QCOMPARE(routes.at(1).block,root.get());
        QCOMPARE(routes.at(1).parseIndex,HeadParserIndex);
        QCOMPARE(routes.at(1).buildIndex,-1);//TODO
        QCOMPARE(routes.at(2).path,"Namespace1Namespace2.cpp");
        QCOMPARE(routes.at(2).block,namespace2);
        QCOMPARE(routes.at(2).parseIndex,SourceParserIndex);
        QCOMPARE(routes.at(2).buildIndex,-1);//TODO
        QCOMPARE(routes.at(3).path,"Namespace1Namespace2.h");
        QCOMPARE(routes.at(3).block,namespace2);
        QCOMPARE(routes.at(3).parseIndex,HeadParserIndex);
        QCOMPARE(routes.at(3).buildIndex,-1);//TODO
    }
    catch (Exception::LogicalRoute& e)
    {
        qDebug() << tr("Logical Route Exception: %1").arg(e.message());
        QVERIFY(false);
    }
}

void TestCppRouter::routes4()
{
    std::unique_ptr<Namespace> root(qobject_cast<Namespace*>(_language->createRootBlock()));
    QVERIFY(root);
    auto namespace1 = appendBlock<Namespace,NamespaceIndex>(root.get());
    auto namespace2 = appendBlock<Namespace,NamespaceIndex>(root.get());
    auto class1 = appendBlock<Class,ClassIndex>(namespace1);
    auto class2 = appendBlock<Class,ClassIndex>(namespace2);
    namespace1->setName("Namespace1");
    namespace2->setName("Namespace2");
    class1->setName("Test");
    class2->setName("Test");
    try
    {
        auto routes = _router->routes(root.get());
        std::sort(
            routes.begin()
            ,routes.end()
            ,[] (const Shape::Route& a, const Shape::Route& b)
            {
                return a.path <= b.path;
            }
            );
        QCOMPARE(routes.size(),4);
        QCOMPARE(routes.at(0).path,"Namespace1.h");
        QCOMPARE(routes.at(0).block,namespace1);
        QCOMPARE(routes.at(0).parseIndex,HeadParserIndex);
        QCOMPARE(routes.at(0).buildIndex,-1);//TODO
        QCOMPARE(routes.at(1).path,"Namespace1Test.h");
        QCOMPARE(routes.at(1).block,class1);
        QCOMPARE(routes.at(1).parseIndex,HeadParserIndex);
        QCOMPARE(routes.at(1).buildIndex,-1);//TODO
        QCOMPARE(routes.at(2).path,"Namespace2.h");
        QCOMPARE(routes.at(2).block,namespace2);
        QCOMPARE(routes.at(2).parseIndex,HeadParserIndex);
        QCOMPARE(routes.at(2).buildIndex,-1);//TODO
        QCOMPARE(routes.at(3).path,"Namespace2Test.h");
        QCOMPARE(routes.at(3).block,class2);
        QCOMPARE(routes.at(3).parseIndex,HeadParserIndex);
        QCOMPARE(routes.at(3).buildIndex,-1);//TODO
    }
    catch (Exception::LogicalRoute& e)
    {
        qDebug() << tr("Logical Route Exception: %1").arg(e.message());
        QVERIFY(false);
    }
}

void TestCppRouter::cleanupTestCase()
{
    delete _language;
    delete _router;
}

QTEST_MAIN(TestCppRouter)
#include "test.moc"
