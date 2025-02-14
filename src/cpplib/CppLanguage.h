#ifndef CPP_LANGUAGE_H
#define CPP_LANGUAGE_H
#include "AbstractLanguage.h"
#include "Cpp.h"
namespace Cpp {




/*!
 * This is a language class. It implements the C++ language.
 */
class Language:
    public AbstractLanguage
{
    Q_OBJECT
    Router* _router;


    public:
    Language(
    );


    public:
    virtual AbstractBlock* createBlock(
        int index
        ,QObject* parent = nullptr
    ) const override final;


    public:
    virtual AbstractParser* createParser(
        int index
    ) const override final;


    public:
    virtual AbstractBlock* createRootBlock(
        QObject* parent = nullptr
    ) const override final;


    public:
    virtual int rootIndex(
    ) const override final;


    public:
    virtual AbstractRouter* router(
    ) const override final;
};
}


#endif
