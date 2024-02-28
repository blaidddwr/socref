#include "BlockCppEnumeration.h"
#include <QtGui>
#include "WidgetBlockCppEnumeration.h"
namespace Block {
namespace Cpp {


Enumeration::Enumeration(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("enumeration",meta,parent)
{
}


Widget::Block::Abstract* Enumeration::createWidget(
) const
{
    return new Widget::Block::Cpp::Enumeration(this);
}


QIcon Enumeration::displayIcon(
) const
{
    return QIcon(":/cpp/enumeration.svg");
}


QString Enumeration::displayText(
) const
{
    auto ret = Base::displayText();
    if (_class)
    {
        ret += " -> class";
    }
    return ret;
}


bool Enumeration::isClass(
) const
{
    return _class;
}


void Enumeration::loadFromMap(
    const QMap<QString,QVariant>& map
    ,int version
)
{
    Base::loadFromMap(map,version);
    _class = map.value("class").toBool();
}


QMap<QString,QVariant> Enumeration::saveToMap(
) const
{
    auto ret = Base::saveToMap();
    if (_class)
    {
        ret.insert("class",true);
    }
    return ret;
}


void Enumeration::setClass(
    bool value
)
{
    if (_class != value)
    {
        _class = value;
        emit classChanged(value);
        emit displayTextChanged(displayText());
    }
}


void Enumeration::setState(
    const QHash<QString,QVariant>& state
)
{
    Base::setState(state);
    setClass(state.value("class").toBool());
}


QHash<QString,QVariant> Enumeration::state(
) const
{
    auto ret = Base::state();
    ret.insert("class",_class);
    return ret;
}


Block::Abstract* Enumeration::create(
    QObject* parent
) const
{
    return new Enumeration(meta(),parent);
}
}
}
