#include "BlockTestNode.h"
#include <QtGui>
namespace Block {
namespace Test {


Widget::Block::Abstract* Node::createWidget(
    QObject* parent
) const
{
    Q_UNUSED(parent);
    return nullptr;//TODO
}


QIcon Node::displayIcon(
) const
{
    return QIcon();
}


QString Node::displayText(
) const
{
    return _name+" display";
}


const QString& Node::name(
) const
{
    return _name;
}


void Node::loadFromMap(
    const QMap<QString,QVariant>& map
    ,int version
)
{
    Q_UNUSED(version);
    _name = map.value("name").toString();
}


QMap<QString,QVariant> Node::saveToMap(
) const
{
    return QMap<QString,QVariant> {{"name",_name}};
}


QString Node::scope(
) const
{
    QStringList names {_name};
    auto block = qobject_cast<Node*>(parent());
    while (block)
    {
        names.prepend(block->name());
        block = qobject_cast<Node*>(parent());
    }
    return names.join(":");
}


void Node::setName(
    const QString& value
)
{
    if (_name != value)
    {
        _name = value;
        emit displayTextChanged(displayText());
    }
}
}
}
