#ifndef TEST_DUMMY_LANGUAGE_H
#define TEST_DUMMY_LANGUAGE_H
#include "AbstractLanguage.h"
#include "ModelMetaBlock.h"
#include "TestBlock.h"

class TestLanguage: public AbstractLanguage
{
    Q_OBJECT
public:
    TestLanguage(Model::Meta::Language* meta, QObject* parent = nullptr):
        AbstractLanguage(meta,parent)
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
        ) const override final
    {
        Q_UNUSED(index);
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
        return nullptr;
    }
};

#endif
