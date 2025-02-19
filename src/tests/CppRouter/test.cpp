#include <QtTest>
#include "CppBlock.h"
#include "CppBlockClass.h"
#include "CppBlockFunction.h"
#include "CppBlockNamespace.h"
#include "CppLanguage.h"
#include "CppParse.h"
#include "CppRouter.h"
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

void TestCppRouter::routes2()
{
    std::unique_ptr<Namespace> root(qobject_cast<Namespace*>(_language->createRootBlock()));
    QVERIFY(root);
    auto nb = appendBlock<Namespace,NamespaceIndex>(root.get());
    auto cb = appendBlock<Class,ClassIndex>(nb);
    appendBlock<Function,FunctionIndex>(cb);
    nb->setName("TestNamespace");
    cb->setName("TestClass");
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
    QCOMPARE(routes.at(0).path,"TestClass.cpp");
    QCOMPARE(routes.at(0).block,cb);
    QCOMPARE(routes.at(0).parseIndex,-1);//TODO
    QCOMPARE(routes.at(0).buildIndex,-1);//TODO
    QCOMPARE(routes.at(1).path,"TestClass.h");
    QCOMPARE(routes.at(1).block,cb);
    QCOMPARE(routes.at(1).parseIndex,HeadParserIndex);
    QCOMPARE(routes.at(1).buildIndex,-1);//TODO
    QCOMPARE(routes.at(2).path,"TestNamespace.h");
    QCOMPARE(routes.at(2).block,nb);
    QCOMPARE(routes.at(2).parseIndex,HeadParserIndex);
    QCOMPARE(routes.at(2).buildIndex,-1);//TODO
}

void TestCppRouter::routes3()
{
    std::unique_ptr<Namespace> root(qobject_cast<Namespace*>(_language->createRootBlock()));
    QVERIFY(root);
    auto nb1 = appendBlock<Namespace,NamespaceIndex>(root.get());
    auto nb2 = appendBlock<Namespace,NamespaceIndex>(nb1);
    appendBlock<Function,FunctionIndex>(root.get());
    appendBlock<Function,FunctionIndex>(nb2);
    nb1->setName("INVISIBLE");
    nb2->setName("Test");
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
    QCOMPARE(routes.at(0).parseIndex,-1);//TODO
    QCOMPARE(routes.at(0).buildIndex,-1);//TODO
    QCOMPARE(routes.at(1).path,"Global.h");
    QCOMPARE(routes.at(1).block,root.get());
    QCOMPARE(routes.at(1).parseIndex,HeadParserIndex);
    QCOMPARE(routes.at(1).buildIndex,-1);//TODO
    QCOMPARE(routes.at(2).path,"Test.cpp");
    QCOMPARE(routes.at(2).block,nb2);
    QCOMPARE(routes.at(2).parseIndex,-1);//TODO
    QCOMPARE(routes.at(2).buildIndex,-1);//TODO
    QCOMPARE(routes.at(3).path,"Test.h");
    QCOMPARE(routes.at(3).block,nb2);
    QCOMPARE(routes.at(3).parseIndex,HeadParserIndex);
    QCOMPARE(routes.at(3).buildIndex,-1);//TODO
}

void TestCppRouter::cleanupTestCase()
{
    delete _language;
    delete _router;
}

QTEST_MAIN(TestCppRouter)
#include "test.moc"
