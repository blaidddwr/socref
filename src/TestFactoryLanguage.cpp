#include "TestFactoryLanguage.h"
#include <QtTest>
#include "FactoryLanguage.h"
#include "LanguageCpp.h"
#include "LanguageCppQt.h"
#include "LanguageTest.h"
#include "ModelMetaLanguage.h"
namespace Test {
namespace Factory {


void Language::initTestCase(
)
{
    _factory = ::Factory::Language::instance();
    QVERIFY(_factory);
    _cppIndex = _factory->indexFromName("cpp");
    _cppQtIndex = _factory->indexFromName("cppqt");
    _testIndex = _factory->indexFromName("test");
    QVERIFY(_cppIndex >= 0);
    QVERIFY(_cppQtIndex >= 0);
    QVERIFY(_testIndex >= 0);
    QCOMPARE(QSet<int>({_cppIndex,_cppQtIndex,_testIndex}).size(),3);
}


void Language::get(
)
{
    QVERIFY(qobject_cast<::Language::Cpp*>(_factory->get(_cppIndex)));
    QVERIFY(qobject_cast<::Language::CppQt*>(_factory->get(_cppQtIndex)));
    QVERIFY(qobject_cast<::Language::Test*>(_factory->get(_testIndex)));
}


void Language::isHidden(
)
{
    QCOMPARE(_factory->isHidden(_cppIndex),false);
    QCOMPARE(_factory->isHidden(_cppQtIndex),false);
    QCOMPARE(_factory->isHidden(_testIndex),true);
}


void Language::meta(
)
{
    auto meta = _factory->meta(_cppIndex);
    QCOMPARE(meta->name(),"cpp");
    QCOMPARE(meta->label(),"C++");
    meta = _factory->meta(_cppQtIndex);
    QCOMPARE(meta->name(),"cppqt");
    QCOMPARE(meta->label(),"C++/Qt");
    meta = _factory->meta(_testIndex);
    QCOMPARE(meta->name(),"test");
    QCOMPARE(meta->label(),"Test");
}


void Language::size(
)
{
    QCOMPARE(_factory->size(),3);
}
}
}
