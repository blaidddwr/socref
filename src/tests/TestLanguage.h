#ifndef TESTLANGUAGE_H
#define TESTLANGUAGE_H
#include "AbstractLanguage.h"
#include "ModelMetaBlock.h"
#include "TestBlock.h"
#include "TestRouter.h"

class TestLanguage: public AbstractLanguage
{
    Q_OBJECT
    TestRouter* _router;
public:
    TestLanguage(Model::Meta::Language* meta, QObject* parent = nullptr):
        AbstractLanguage(meta,parent)
        ,_router(new TestRouter(this))
    {
        appendBlock(
            new Model::Meta::Block(
                meta
                ,BLOCK_INDEX
                ,BLOCK_NAME
                ,BLOCK_LABEL
                ,QIcon()
                ,{BLOCK_INDEX}
                )
            );
    }
    virtual AbstractBlock* createBlock(int index, QObject* parent = nullptr) const override final
    {
        if (index != BLOCK_INDEX)
        {
            return nullptr;
        }
        return new TestBlock(blockMeta(BLOCK_INDEX),parent);
    }
    virtual AbstractParser* createParser(
        int index
        ,AbstractBlock* block
        ,int version
        ) const override final
    {
        Q_UNUSED(index);
        Q_UNUSED(block);
        Q_UNUSED(version);
        return nullptr;
    }
    virtual AbstractBlock* createRootBlock(QObject* parent = nullptr) const override final
    {
        return new TestBlock(blockMeta(BLOCK_INDEX),parent);
    }
    virtual int rootIndex() const override final
    {
        return BLOCK_INDEX;
    }
    virtual AbstractRouter* router() const override final
    {
        return _router;
    }
};

#endif
