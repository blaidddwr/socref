#include "FactoryLanguage.h"
#include <QtGui>
#include "AbstractLanguage.h"
#include "ModelMetaLanguage.h"
namespace Factory {
Language* Language::_instance {nullptr};


void Language::appendLanguage(
    AbstractLanguage* language
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


AbstractLanguage* Language::get(
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
}
}
