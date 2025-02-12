#include <QtTest>
#include "Factory.h"
#include "FactoryLanguage.h"
#include "ModelMetaLanguage.h"
#include "../TestLanguage.h"
#include "../utility.h"

class TestFactoryLanguage: public QObject
{
    Q_OBJECT
    AbstractLanguage* _lang1;
    AbstractLanguage* _lang2;
    Factory::Language* _factory;
    int _index1;
    int _index2;
private slots:
    void initTestCase();
    void get();
    void meta();
    void size();
};

void TestFactoryLanguage::initTestCase()
{
    Q_INIT_RESOURCE(core);
    _factory = ::Factory::Language::instance();
    QVERIFY(_factory);
    _lang1 = new TestLanguage(new Model::Meta::Language("lang1","Language 1",QIcon()),this);
    _lang2 = new TestLanguage(
        new Model::Meta::Language("lang2","Language 2",QIcon(":/application.svg")),this
        );
    _factory->appendLanguage(_lang1);
    _factory->appendLanguage(_lang2);
    QCOMPARE(_lang1->parent(),_factory);
    QCOMPARE(_lang2->parent(),_factory);
    _index1 = _factory->indexFromName("lang1");
    _index2 = _factory->indexFromName("lang2");
    QVERIFY(_index1 >= 0);
    QVERIFY(_index2 >= 0);
}

void TestFactoryLanguage::get()
{
    QCOMPARE(_factory->get(_index1),_lang1);
    QCOMPARE(_factory->get(_index2),_lang2);
}

void TestFactoryLanguage::meta()
{
    static const QIcon testLang1Icon;
    static const QIcon testLang2Icon(":/application.svg");
    auto meta = _factory->meta(_index1);
    QCOMPARE(meta->name(),"lang1");
    QCOMPARE(meta->label(),"Language 1");
    QVERIFY(areIconsEqual(meta->displayIcon(),testLang1Icon));
    meta = _factory->meta(_index2);
    QCOMPARE(meta->name(),"lang2");
    QCOMPARE(meta->label(),"Language 2");
    QVERIFY(areIconsEqual(meta->displayIcon(),testLang2Icon));
}

void TestFactoryLanguage::size()
{
    QCOMPARE(_factory->size(),2);
}

QTEST_MAIN(TestFactoryLanguage)
#include "test.moc"
