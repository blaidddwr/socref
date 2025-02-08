#ifndef LANGUAGE_CPP_H
#define LANGUAGE_CPP_H
#include "LanguageAbstract.h"
namespace Language {




/*!
 * This is a language class. It implements the C++ language.
 */
class Cpp:
    public Language::Abstract
{
    Q_OBJECT


    public:
    Cpp(
        Model::Meta::Language* meta
        ,QObject* parent = nullptr
    );


    public:
    virtual Block::Abstract* createBlock(
        int index
        ,QObject* parent = nullptr
    ) const override final;


    public:
    virtual Controller::Parse::Abstract* createParser(
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
    virtual Controller::Route::Abstract* router(
    ) const override final;
};
}


#endif
