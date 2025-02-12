#include <QtTest>
#include "ModelMeta.h"
#include "../TestLanguage.h"

class TestAbstractLanguage: public QObject
{
    Q_OBJECT
    AbstractLanguage* _language;
    Model::Meta::Language* _meta;
private slots:
    void initTestCase();
    void blockMeta();
    void indexFromName();
    void metaProperty();
    void size();
    void cleanupTestCase();
};

void TestAbstractLanguage::initTestCase()
{
    _meta = new Model::Meta::Language(LANGUAGE_NAME,LANGUAGE_LABEL,QIcon(),this);
    _language = new TestLanguage(_meta,this);
}

void TestAbstractLanguage::blockMeta()
{
    auto meta = _language->blockMeta(BLOCK_INDEX);
    QCOMPARE(meta->name(),BLOCK_NAME);
    QCOMPARE(meta->label(),BLOCK_LABEL);
    QCOMPARE(meta->index(),BLOCK_INDEX);
    QCOMPARE(meta->language(),_meta);
    QCOMPARE(meta->allowList(),{BLOCK_INDEX});
}

void TestAbstractLanguage::indexFromName()
{
    QCOMPARE(_language->indexFromName(BLOCK_NAME),BLOCK_INDEX);
    QCOMPARE(_language->indexFromName("does_not_exist"),-1);
}

void TestAbstractLanguage::metaProperty()
{
    QCOMPARE(_language->meta(),_meta);
}

void TestAbstractLanguage::size()
{
    QCOMPARE(_language->size(),1);
}

void TestAbstractLanguage::cleanupTestCase()
{
    delete _language;
}

QTEST_MAIN(TestAbstractLanguage)
#include "test.moc"
