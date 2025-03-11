#include "CppBlockEnumeration.h"
#include <QtGui>
#include "CppBlockWidgetEnumerationEdit.h"
#define CLASS "class"
namespace Cpp {
namespace Block {


Enumeration::Enumeration(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Namespace("enumeration",meta,parent)
{
}


AbstractBlockWidget* Enumeration::createWidget(
) const
{
    return new BlockWidget::EnumerationEdit(this);
}


QIcon Enumeration::displayIcon(
) const
{
    return QIcon(":/cpp/enumeration.svg");
}


QString Enumeration::displayText(
) const
{
    auto ret = Namespace::displayText();
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
    Namespace::loadFromMap(map,version);
    _class = map.value(CLASS).toBool();
}


QMap<QString,QVariant> Enumeration::saveToMap(
) const
{
    auto ret = Namespace::saveToMap();
    if (_class)
    {
        ret.insert(CLASS,true);
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
    Namespace::setState(state);
    setClass(state.value(CLASS).toBool());
}


QHash<QString,QVariant> Enumeration::state(
) const
{
    auto ret = Namespace::state();
    ret.insert(CLASS,_class);
    return ret;
}


AbstractBlock* Enumeration::create(
    QObject* parent
) const
{
    return new Enumeration(meta(),parent);
}
}
}
