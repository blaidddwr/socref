#include "FactoryLanguage.h"
#include <QtGui>
#include "LanguageCpp.h"
#include "LanguageCppQt.h"
#include "ModelMetaLanguage.h"
#include "gassert.h"
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


Language* Language::instance(
)
{
    if (!_instance)
    {
        _instance = new Language;
    }
    return _instance;
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
    appendLanguage(new Cpp(new Model::Meta::Language("cpp","C++",QIcon(":/cpp.svg"))));
    appendLanguage(new CppQt(new Model::Meta::Language("cppqt","C++/Qt",QIcon(":/cppqt.svg"))));
}


void Language::appendLanguage(
    ::Language::Abstract* language
)
{
    G_ASSERT(language);
    auto meta = language->meta();
    G_ASSERT(!_lookup.contains(meta->name()));
    meta->setParent(this);
    language->setParent(this);
    _lookup.insert(meta->name(),_languages.size());
    _languages.append(language);
}
}
