#include "ModelMetaLanguage.h"
#include <QtGui>
namespace Model {
namespace Meta {


Language::Language(
    const QString& name
    ,const QString& label
    ,const QIcon& displayIcon
    ,QObject* parent
):
    QObject(parent)
    ,_displayIcon(new QIcon(displayIcon))
    ,_label(label)
    ,_name(name)
{
}


Language::~Language(
)
{
    delete _displayIcon;
}


const QIcon& Language::displayIcon(
) const
{
    return *_displayIcon;
}


const QString& Language::label(
) const
{
    return _label;
}


const QString& Language::name(
) const
{
    return _name;
}
}
}
