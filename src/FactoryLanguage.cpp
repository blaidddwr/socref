#include "FactoryLanguage.h"
#include <QtCore>
#include "LanguageCpp.h"
#include "ModelMetaLanguage.h"
namespace Factory {
//#include "LanguageCppQt.h"
Language* Language::_instance {nullptr};


::Language::Abstract* Language::get(
    int index
) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < _languages.size());
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


Model::Meta::Language* Language::meta(
    int index
) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < _languages.size());
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
    //appendLanguage(new CppQt(new Model::Meta::Language("cppqt","C++/Qt")));
}


void Language::appendLanguage(
    ::Language::Abstract* language
)
{
    Q_ASSERT(language);
    auto meta = language->meta();
    Q_ASSERT(!_lookup.contains(meta->name()));
    meta->setParent(this);
    language->setParent(this);
    _lookup.insert(meta->name(),_languages.size());
    _languages.append(language);
}
}
