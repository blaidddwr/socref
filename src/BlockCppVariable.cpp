#include "BlockCppVariable.h"
#include <QtGui>
#include "BlockCppFunction.h"
namespace Block {
namespace Cpp {


Variable::Variable(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("variable",meta,parent)
{
}


const QString& Variable::assignment(
) const
{
    return _assignment;
}


Widget::Block::Abstract* Variable::createWidget(
    QObject* parent
) const
{
    Q_UNUSED(parent);
    return nullptr;//TODO
}


QIcon Variable::displayIcon(
) const
{
    return QIcon(":/cpp/variable.svg");
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


const QString& Variable::type(
) const
{
    return _type;
}


void Variable::addEvent(
)
{
    if (auto p = qobject_cast<Function*>(parent()))
    {
        p->updateDisplayText();
    }
}


void Variable::removeEvent(
)
{
    if (auto p = qobject_cast<Function*>(parent()))
    {
        p->updateDisplayText();
    }
}
}
}
