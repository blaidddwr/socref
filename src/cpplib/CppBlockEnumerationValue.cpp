#include "CppBlockEnumerationValue.h"
#include <QtGui>
#include "CppBlockWidgetEnumerationValueEdit.h"
#define ASSIGNMENT "assignment"
namespace Cpp {
namespace Block {


EnumerationValue::EnumerationValue(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Namespace("value",meta,parent)
{
}


const QString& EnumerationValue::assignment(
) const
{
    return _assignment;
}


AbstractBlockWidget* EnumerationValue::createWidget(
) const
{
    return new BlockWidget::EnumerationValueEdit(this);
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
    Namespace::loadFromMap(map,version);
    _assignment = map.value(ASSIGNMENT).toString();
}


QMap<QString,QVariant> EnumerationValue::saveToMap(
) const
{
    auto ret = Namespace::saveToMap();
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
    Namespace::setState(state);
    setAssignment(state.value(ASSIGNMENT).toString());
}


QHash<QString,QVariant> EnumerationValue::state(
) const
{
    auto ret = Namespace::state();
    ret.insert(ASSIGNMENT,_assignment);
    return ret;
}


AbstractBlock* EnumerationValue::create(
    QObject* parent
) const
{
    return new EnumerationValue(meta(),parent);
}
}
}
