#include "BlockCppClass.h"
#include <QtGui>
#include "Global.h"
namespace Block {
namespace Cpp {


Widget::Block::Abstract* Class::createWidget(
    QObject* parent
) const
{
    //TODO
    Q_UNUSED(parent);
    return nullptr;
}


QIcon Class::displayIcon(
) const
{
    return QIcon(":/cpp/class.svg");
}


void Class::loadFromMap(
    const QMap<QString,QVariant>& map
    ,int version
)
{
    Base::loadFromMap(map,version);
    if (version == Socref_Legacy)
    {
        _parents = map.value("parents").toString().split('\n',Qt::SkipEmptyParts);
    }
    else
    {
        _parents = map.value("parents").toString().split(';');
    }
    _template = map.value("template").toString();
}


const QStringList& Class::parents(
) const
{
    return _parents;
}


QMap<QString,QVariant> Class::saveToMap(
) const
{
    auto ret = Base::saveToMap();
    if (!_parents.isEmpty())
    {
        ret.insert("parents",_parents.join(';'));
    }
    if (!_template.isEmpty())
    {
        ret.insert("template",_template);
    }
    return ret;
}


void Class::setParents(
    const QStringList& value
)
{
    if (_parents != value)
    {
        _parents = value;
        emit parentsChanged(value);
    }
}


void Class::setTemplateString(
    const QString& value
)
{
    if (_template != value)
    {
        _template = value;
        emit templateStringChanged(value);
    }
}


const QString& Class::templateString(
) const
{
    return _template;
}
}
}
