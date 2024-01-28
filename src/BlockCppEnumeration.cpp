#include "BlockCppEnumeration.h"
#include <QtGui>
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
    QObject* parent
) const
{
    //TODO
    Q_UNUSED(parent);
    return nullptr;
}


QIcon Enumeration::displayIcon(
) const
{
    return QIcon(":/cpp/enumeration.svg");
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
