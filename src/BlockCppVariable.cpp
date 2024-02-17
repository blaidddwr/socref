#include "BlockCppVariable.h"
#include <QtGui>
#include "BlockCppFunction.h"
#include "Exceptions.h"
namespace Block {
namespace Cpp {


Variable::Variable(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("variable",meta,parent)
{
}


Variable::~Variable(
)
{
    if (auto p = qobject_cast<Function*>(parent()))
    {
        auto index = p->indexOf(this);
        G_ASSERT(index != -1);
        p->take(index);
    }
}


const QString& Variable::assignment(
) const
{
    return _assignment;
}


Widget::Block::Abstract* Variable::createWidget(
) const
{
    return nullptr;//TODO
}


QIcon Variable::displayIcon(
) const
{
    return QIcon(":/cpp/variable.svg");
}


void Variable::loadFromMap(
    const QMap<QString,QVariant>& map
    ,int version
)
{
    Base::loadFromMap(map,version);
    _type = map.value("type").toString();
    _assignment = map.value("assignment").toString();
}


QMap<QString,QVariant> Variable::saveToMap(
) const
{
    auto ret = Base::saveToMap();
    ret.insert("type",_type);
    if (!_assignment.isEmpty())
    {
        ret.insert("assignment",_assignment);
    }
    return ret;
}


void Variable::setAssignment(
    const QString& value
)
{
    if (_assignment != value)
    {
        _assignment = value;
        emit assignmentChanged(value);
    }
}


void Variable::setState(
    const QHash<QString,QVariant>& state
)
{
    Base::setState(state);
    setType(state.value("type").toString());
    setAssignment(state.value("assignment").toString());
}


void Variable::setType(
    const QString& value
)
{
    if (_type != value)
    {
        _type = value;
        emit typeChanged(value);
        if (auto p = qobject_cast<Function*>(parent()))
        {
            p->updateDisplayText();
        }
    }
}


QHash<QString,QVariant> Variable::state(
) const
{
    auto ret = Base::state();
    ret.insert("type",_type);
    ret.insert("assignment",_assignment);
    return ret;
}


const QString& Variable::type(
) const
{
    return _type;
}


void Variable::addEvent(
    int index
)
{
    Q_UNUSED(index);
    if (auto p = qobject_cast<Function*>(parent()))
    {
        p->updateDisplayText();
    }
}


Block::Abstract* Variable::create(
    QObject* parent
) const
{
    return new Variable(meta(),parent);
}


void Variable::moveEvent(
    int from
    ,int to
)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    if (auto p = qobject_cast<Function*>(parent()))
    {
        p->updateDisplayText();
    }
}


void Variable::removeEvent(
    int index
)
{
    Q_UNUSED(index);
    if (auto p = qobject_cast<Function*>(parent()))
    {
        p->updateDisplayText();
    }
}
}
}
