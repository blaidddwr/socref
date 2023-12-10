#include "TestLanguageAbstract.h"
#include <QtTest>
#include "BlockTest.h"
#include "BlockTestNode.h"
#include "LanguageTest.h"
#include "ModelMetaBlock.h"
#include "ModelMetaLanguage.h"
namespace Test {
namespace Language {


void Abstract::initTestCase(
)
{
    _meta = new Model::Meta::Language("test","Test",this);
    _language = new ::Language::Test(_meta,this);
}


void Abstract::blockMeta(
)
{
    using namespace Block::Test;
    static const QSet<int> allowList {NodeIndex};
    auto meta = _language->blockMeta(NodeIndex);
    QCOMPARE(meta->name(),"node");
    QCOMPARE(meta->label(),"Node");
    QCOMPARE(meta->index(),NodeIndex);
    QCOMPARE(meta->language(),_meta);
    QCOMPARE(meta->allowList(),allowList);
}


void Abstract::create(
)
{
    using namespace Block::Test;
    auto node = qobject_cast<Node*>(_language->create(NodeIndex,this));
    QVERIFY(node);
    QCOMPARE(node->meta()->language(),_meta);
    delete node;
}


void Abstract::indexFromName(
)
{
    using namespace Block::Test;
    QCOMPARE(_language->indexFromName("node"),NodeIndex);
    QCOMPARE(_language->indexFromName("does_not_exist"),-1);
}


void Abstract::metaProperty(
)
{
    QCOMPARE(_language->meta(),_meta);
}


void Abstract::rootIndexProperty(
)
{
    using namespace Block::Test;
    QCOMPARE(_language->rootIndex(),NodeIndex);
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
