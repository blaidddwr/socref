#include "TestFactoryLanguage.h"
#include <QtTest>
#include "FactoryLanguage.h"
#include "LanguageCpp.h"
#include "LanguageCppQt.h"
#include "ModelMetaLanguage.h"
#include "Test.h"
namespace Test {
namespace Factory {


void Language::initTestCase(
)
{
    _factory = ::Factory::Language::instance();
    QVERIFY(_factory);
    _cppIndex = _factory->indexFromName("cpp");
    _cppQtIndex = _factory->indexFromName("cppqt");
    QVERIFY(_cppIndex >= 0);
    QVERIFY(_cppQtIndex >= 0);
}


void Language::get(
)
{
    QVERIFY(qobject_cast<::Language::Cpp*>(_factory->get(_cppIndex)));
    QVERIFY(qobject_cast<::Language::CppQt*>(_factory->get(_cppQtIndex)));
}


void Language::meta(
)
{
    static const QIcon testCppIcon(":/cpp/icon.svg");
    static const QIcon testCppQtIcon(":/cppqt/icon.svg");
    auto meta = _factory->meta(_cppIndex);
    QCOMPARE(meta->name(),"cpp");
    QCOMPARE(meta->label(),"C++");
    QVERIFY(areIconsEqual(meta->displayIcon(),testCppIcon));
    meta = _factory->meta(_cppQtIndex);
    QCOMPARE(meta->name(),"cppqt");
    QCOMPARE(meta->label(),"C++/Qt");
    QVERIFY(areIconsEqual(meta->displayIcon(),testCppQtIcon));
}


void Language::size(
)
{
    QCOMPARE(_factory->size(),2);
}
}
}
