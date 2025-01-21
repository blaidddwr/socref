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
    virtual Block::Abstract* create(
        int index
        ,QObject* parent = nullptr
    ) const override final;


    public:
    virtual Block::Abstract* createRoot(
        QObject* parent = nullptr
    ) const override final;


    public:
    virtual int rootIndex(
    ) const override final;
};
}


#endif
