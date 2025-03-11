#ifndef CPPQT_LANGUAGE_H
#define CPPQT_LANGUAGE_H
#include "CppLanguage.h"
namespace CppQt {




/*!
 * This is a language class. It implements the C++/Qt language.
 */
class Language:
    public Cpp::Language
{
    Q_OBJECT


    public:
    Language(
    );


    public:
    virtual AbstractBlock* createBlock(
        int index
        ,QObject* parent = nullptr
    ) const override final;
};
}


#endif
