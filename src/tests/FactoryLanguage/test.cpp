#include <QtTest>
#include "Factory.h"
#include "FactoryLanguage.h"
#include "LanguageCpp.h"
#include "LanguageCppQt.h"
#include "ModelMetaLanguage.h"
#include "../utility.h"

class TestLanguageFactory: public QObject
{
    Q_OBJECT
    Factory::Language* _factory;
    int _cppIndex;
    int _cppQtIndex;
private slots:
    void initTestCase();
    void get();
    void meta();
    void size();
};

void TestLanguageFactory::initTestCase()
{
    Q_INIT_RESOURCE(resources);
    _factory = ::Factory::Language::instance();
    QVERIFY(_factory);
    _cppIndex = _factory->indexFromName("cpp");
    _cppQtIndex = _factory->indexFromName("cppqt");
    QVERIFY(_cppIndex >= 0);
    QVERIFY(_cppQtIndex >= 0);
}

void TestLanguageFactory::get()
{
    QVERIFY(qobject_cast<::Language::Cpp*>(_factory->get(_cppIndex)));
    QVERIFY(qobject_cast<::Language::CppQt*>(_factory->get(_cppQtIndex)));
}

void TestLanguageFactory::meta()
{
    static const QIcon testCppIcon(":/cpp.svg");
    static const QIcon testCppQtIcon(":/cppqt.svg");
    auto meta = _factory->meta(_cppIndex);
    QCOMPARE(meta->name(),"cpp");
    QCOMPARE(meta->label(),"C++");
    QVERIFY(areIconsEqual(meta->displayIcon(),testCppIcon));
    meta = _factory->meta(_cppQtIndex);
    QCOMPARE(meta->name(),"cppqt");
    QCOMPARE(meta->label(),"C++/Qt");
    QVERIFY(areIconsEqual(meta->displayIcon(),testCppQtIcon));
}

void TestLanguageFactory::size()
{
    QCOMPARE(_factory->size(),2);
}

QTEST_MAIN(TestLanguageFactory)
#include "test.moc"
