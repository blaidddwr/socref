#include "BlockTestNode.h"
#include <QtGui>
namespace Block {
namespace Test {


Node::~Node(
)
{
    delete _icon;
}


int Node::addCount(
) const
{
    return _addCount;
}


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
    if (!_icon)
    {
        return QIcon();
    }
    else
    {
        return *_icon;
    }
}


QString Node::displayText(
) const
{
    return _name;
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


int Node::removeCount(
) const
{
    return _removeCount;
}


void Node::resetAdd(
)
{
    _addCount = 0;
}


void Node::resetRemove(
)
{
    _removeCount = 0;
}


QMap<QString,QVariant> Node::saveToMap(
) const
{
    return QMap<QString,QVariant> {{"name",_name}};
}


QString Node::scope(
) const
{
    auto scope = _name;
    scope.replace("\n","");
    QStringList names {scope};
    auto block = qobject_cast<Node*>(parent());
    if (!block)
    {
        return "ROOT";
    }
    while (block)
    {
        auto next = qobject_cast<Node*>(block->parent());
        if (next)
        {
            scope = block->name();
            scope.replace("\n","");
            names.prepend(scope);
        }
        block = next;
    }
    return names.join(":");
}


void Node::setIcon(
    const QIcon& icon
)
{
    if (!_icon)
    {
        _icon = new QIcon();
    }
    *_icon = icon;
    emit displayIconChanged(icon);
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


void Node::addEvent(
)
{
    _addCount++;
}


void Node::removeEvent(
)
{
    _removeCount++;
}
}
}
