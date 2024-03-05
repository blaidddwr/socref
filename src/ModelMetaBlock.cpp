#include "ModelMetaBlock.h"
#include <QtGui>
#include "gassert.h"
namespace Model {
namespace Meta {


Block::Block(
    Model::Meta::Language* language
    ,int index
    ,const QString& name
    ,const QString& label
    ,const QIcon* displayIcon
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
    G_ASSERT(displayIcon);
    connect(language,&QObject::destroyed,this,&Block::onLanguageDestroyed);
}


Block::~Block(
)
{
    delete _displayIcon;
}


const QSet<int>& Block::allowList(
) const
{
    return _allowList;
}


QIcon Block::displayIcon(
) const
{
    G_ASSERT(_displayIcon);
    return *_displayIcon;
}


int Block::index(
) const
{
    return _index;
}


Model::Meta::Language* Block::language(
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
