#include "ModelMetaBlock.h"
namespace Model {
namespace Meta {


Block::Block(
    Model::Meta::Language* language
    ,int index
    ,const QString& name
    ,const QString& label
    ,const QSet<int>& allowList
    ,QObject* parent
):
    Language(name,label,parent)
    ,_language(language)
    ,_allowList(allowList)
    ,_index(index)
{
    Q_ASSERT(language);
    connect(language,&QObject::destroyed,this,&Block::onLanguageDestroyed);
}


const QSet<int>& Block::allowList(
) const
{
    return _allowList;
}


int Block::index(
) const
{
    return _index;
}


Model::Meta::Language* Block::language(
) const
{
    Q_ASSERT(_language);
    return _language;
}


void Block::onLanguageDestroyed(
    QObject* object
)
{
    if (_language == object)
    {
        _language = nullptr;
    }
}
}
}
