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
