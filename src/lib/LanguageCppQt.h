#ifndef LANGUAGE_CPPQT_H
#define LANGUAGE_CPPQT_H
#include "LanguageAbstract.h"
namespace Language {




/*!
 * This is a language class. It implements the C++/Qt language.
 */
class CppQt:
    public Language::Abstract
{
    Q_OBJECT


    public:
    CppQt(
        Model::Meta::Language* meta
        ,QObject* parent = nullptr
    );


    public:
    virtual Block::Abstract* createBlock(
        int index
        ,QObject* parent = nullptr
    ) const override final;


    public:
    virtual Controller::Code::AbstractParser* createParser(
        int index
        ,QObject* parent = nullptr
    ) const override final;


    public:
    virtual Block::Abstract* createRootBlock(
        QObject* parent = nullptr
    ) const override final;


    public:
    virtual int rootIndex(
    ) const override final;


    public:
    virtual Controller::Code::AbstractRouter* router(
    ) const override final;
};
}


#endif
