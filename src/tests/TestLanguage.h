#ifndef TEST_DUMMY_LANGUAGE_H
#define TEST_DUMMY_LANGUAGE_H
#include "LanguageAbstract.h"
#include "ModelMetaBlock.h"
#include "TestBlock.h"

class TestLanguage: public Language::Abstract
{
    Q_OBJECT
public:
    TestLanguage(Model::Meta::Language* meta, QObject* parent = nullptr):
        Abstract(meta,parent)
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
    virtual Block::Abstract* createBlock(int index, QObject* parent = nullptr) const override final
    {
        if (index != BLOCK_INDEX)
        {
            return nullptr;
        }
        return new TestBlock(blockMeta(BLOCK_INDEX),parent);
    }
    virtual Controller::Parse::Abstract* createParser(
        int index
        ,QObject *parent) const override final
    {
        Q_UNUSED(index);
        Q_UNUSED(parent);
        return nullptr;
    }
    virtual Block::Abstract* createRootBlock(QObject* parent = nullptr) const override final
    {
        Q_UNUSED(parent);
        return nullptr;
    }
    virtual int rootIndex() const override final
    {
        return BLOCK_INDEX;
    }
    virtual Controller::Route::Abstract* router() const override final
    {
        return nullptr;
    }
};

#endif
