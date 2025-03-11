#ifndef CPP_LANGUAGE_H
#define CPP_LANGUAGE_H
#include "AbstractLanguage.h"
#include "Cpp.h"
namespace Cpp {




/*!
 * This is a language class. It implements the C++ language.
 * 
 * This class can also be extended with an extremely tight set of requirements.
 * Another language can extend the C++ language if it uses all the same block
 * indexes, and properly creates blocks from its own language that extend any
 * C++ blocks. The parsing, root block, and routing must be identical.
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
    ) const override;


    public:
    virtual AbstractParser* createParser(
        int index
        ,AbstractBlock* block
        ,int version
    ) const override;


    public:
    virtual AbstractBlock* createRootBlock(
        QObject* parent = nullptr
    ) const override;


    public:
    virtual int rootIndex(
    ) const override;


    public:
    virtual AbstractRouter* router(
    ) const override final;


    /*!
     * Detailed description.
     *
     * @param meta
     *        Detailed description.
     *
     * @param parent
     *        Detailed description.
     */
    protected:
    Language(
        Model::Meta::Language* meta
        ,QObject* parent = nullptr
    );
};
}


#endif
