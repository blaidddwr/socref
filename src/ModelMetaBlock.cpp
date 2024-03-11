#include "ModelMetaBlock.h"
#include <QtGui>
#include "gassert.h"
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
    Language(name,label,parent)
    ,_language(language)
    ,_displayIcon(displayIcon)
    ,_allowList(allowList)
    ,_index(index)
{
    G_ASSERT(language);
    connect(language,&QObject::destroyed,this,&Block::onLanguageDestroyed);
}


const QSet<int>& Block::allowList(
) const
{
    return _allowList;
}


const QIcon& Block::displayIcon(
) const
{
    return _displayIcon;
}


int Block::index(
) const
{
    return _index;
}


Language* Block::language(
) const
{
    G_ASSERT(_language);
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
