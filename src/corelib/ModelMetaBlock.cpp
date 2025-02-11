#include "ModelMetaBlock.h"
#include <QtCore>
namespace Model {
namespace Meta {


Block::Block(
    Language* language
    ,int index
    ,const QString& name
    ,const QString& label
    ,const QIcon& displayIcon
    ,const QSet<int>& allowList
    ,QObject* parent
):
    Language(name,label,displayIcon,parent)
    ,_language(language)
    ,_allowList(new QSet<int>(allowList))
    ,_index(index)
{
    Q_ASSERT(language);
    connect(language,&QObject::destroyed,this,&Block::onLanguageDestroyed);
}


Block::~Block(
)
{
    delete _allowList;
}


const QSet<int>& Block::allowList(
) const
{
    return *_allowList;
}


int Block::index(
) const
{
    return _index;
}


Language* Block::language(
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
