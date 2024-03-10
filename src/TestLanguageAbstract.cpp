#include "TestLanguageAbstract.h"
#include <QtTest>
#include "TestDummyLanguage.h"
namespace Test {
namespace Language {
using Dummy = Dummy::Language;


void Abstract::initTestCase(
)
{
    _meta = new Model::Meta::Language(LANGUAGE_NAME,LANGUAGE_LABEL,this);
    _language = new Dummy(_meta,this);
}


void Abstract::blockMeta(
)
{
    auto meta = _language->blockMeta(BLOCK_INDEX);
    QCOMPARE(meta->name(),BLOCK_NAME);
    QCOMPARE(meta->label(),BLOCK_LABEL);
    QCOMPARE(meta->index(),BLOCK_INDEX);
    QCOMPARE(meta->language(),_meta);
    QCOMPARE(meta->allowList(),{BLOCK_INDEX});
}


void Abstract::indexFromName(
)
{
    QCOMPARE(_language->indexFromName(BLOCK_NAME),BLOCK_INDEX);
    QCOMPARE(_language->indexFromName("does_not_exist"),-1);
}


void Abstract::metaProperty(
)
{
    QCOMPARE(_language->meta(),_meta);
}


void Abstract::size(
)
{
    QCOMPARE(_language->size(),1);
}


void Abstract::cleanupTestCase(
)
{
    delete _meta;
    delete _language;
}
}
}
