#include "BlockCppEnumerationValue.h"
#include <QtGui>
#include "WidgetBlockCppEnumerationValue.h"
#define ASSIGNMENT "assignment"
namespace Block {
namespace Cpp {


EnumerationValue::EnumerationValue(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("value",meta,parent)
{
}


const QString& EnumerationValue::assignment(
) const
{
    return _assignment;
}


Widget::Block::Abstract* EnumerationValue::createWidget(
) const
{
    return new Widget::Block::Cpp::EnumerationValue(this);
}


QIcon EnumerationValue::displayIcon(
) const
{
    return QIcon(":/cpp/enumeration_value.svg");
}


void EnumerationValue::loadFromMap(
    const QMap<QString,QVariant>& map
    ,int version
)
{
    Base::loadFromMap(map,version);
    _assignment = map.value(ASSIGNMENT).toString();
}


QMap<QString,QVariant> EnumerationValue::saveToMap(
) const
{
    auto ret = Base::saveToMap();
    if (!_assignment.isEmpty())
    {
        ret.insert(ASSIGNMENT,_assignment);
    }
    return ret;
}


void EnumerationValue::setAssignment(
    const QString& value
)
{
    if (_assignment != value)
    {
        _assignment = value;
        emit assignmentChanged(value);
    }
}


void EnumerationValue::setState(
    const QHash<QString,QVariant>& state
)
{
    Base::setState(state);
    setAssignment(state.value(ASSIGNMENT).toString());
}


QHash<QString,QVariant> EnumerationValue::state(
) const
{
    auto ret = Base::state();
    ret.insert(ASSIGNMENT,_assignment);
    return ret;
}


Abstract* EnumerationValue::create(
    QObject* parent
) const
{
    return new EnumerationValue(meta(),parent);
}
}
}
