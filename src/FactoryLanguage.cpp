#include "FactoryLanguage.h"
#include <QtCore>
#include "Exceptions.h"
#include "LanguageCpp.h"
#include "LanguageCppQt.h"
#include "LanguageTest.h"
#include "ModelMetaLanguage.h"
namespace Factory {
Language* Language::_instance {nullptr};


::Language::Abstract* Language::get(
    int index
) const
{
    G_ASSERT(index >= 0);
    G_ASSERT(index < _languages.size());
    return _languages.at(index);
}


int Language::indexFromName(
    const QString& name
) const
{
    return _lookup.value(name,-1);
}


Factory::Language* Language::instance(
)
{
    if (!_instance)
    {
        _instance = new Language;
    }
    return _instance;
}


bool Language::isHidden(
    int index
) const
{
    G_ASSERT(index >= 0);
    G_ASSERT(index < _isHiddenList.size());
    return _isHiddenList.at(index);
}


Model::Meta::Language* Language::meta(
    int index
) const
{
    G_ASSERT(index >= 0);
    G_ASSERT(index < _languages.size());
    return _languages.at(index)->meta();
}


int Language::size(
) const
{
    return _languages.size();
}


Language::Language(
):
    QObject(QCoreApplication::instance())
{
    using namespace Language;
    appendLanguage(new Cpp(new Model::Meta::Language("cpp","C++")));
    appendLanguage(new CppQt(new Model::Meta::Language("cppqt","C++/Qt")));
    appendLanguage(new Test(new Model::Meta::Language("test","Test")),true);
}


void Language::appendLanguage(
    ::Language::Abstract* language
    ,bool isHidden
)
{
    G_ASSERT(language);
    auto meta = language->meta();
    G_ASSERT(!_lookup.contains(meta->name()));
    meta->setParent(this);
    language->setParent(this);
    _lookup.insert(meta->name(),_languages.size());
    _languages.append(language);
    _isHiddenList.append(isHidden);
}
}
