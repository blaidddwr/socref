#ifndef LANGUAGE_TEST_H
#define LANGUAGE_TEST_H
#include "LanguageAbstract.h"
namespace Language {




/*!
 * This is a language class. It implements a test language.
 */
class Test:
    public Language::Abstract
{
    Q_OBJECT


    public:
    Test(
        Model::Meta::Language* meta
        ,QObject* parent = nullptr
    );


    public:
    virtual Block::Abstract* create(
        int index
        ,QObject* parent = nullptr
    ) const override final;


    public:
    virtual int rootIndex(
    ) const override final;
};
}


#endif