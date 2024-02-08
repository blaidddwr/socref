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


QString Node::filename(
) const
{
    return _name;
}


int Node::lastAddIndex(
) const
{
    return _lastAddIndex;
}


int Node::lastMoveFromIndex(
) const
{
    return _lastMoveFromIndex;
}


int Node::lastMoveToIndex(
) const
{
    return _lastMoveToIndex;
}


int Node::lastRemoveIndex(
) const
{
    return _lastRemoveIndex;
}


void Node::loadFromMap(
    const QMap<QString,QVariant>& map
    ,int version
)
{
    Q_UNUSED(version);
    _name = map.value("name").toString();
}


int Node::moveCount(
) const
{
    return _moveCount;
}


const QString& Node::name(
) const
{
    return _name;
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


void Node::resetMove(
)
{
    _moveCount = 0;
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


void Node::setState(
    const QHash<QString,QVariant>& state
)
{
    setName(state.value("name").toString());
}


QHash<QString,QVariant> Node::state(
) const
{
    return {
        {"name",_name}
    };
}


void Node::addEvent(
    int index
)
{
    _lastAddIndex = index;
    _addCount++;
}


Block::Abstract* Node::create(
    QObject* parent
) const
{
    return new Node(meta(),parent);
}


void Node::moveEvent(
    int from
    ,int to
)
{
    _lastMoveFromIndex = from;
    _lastMoveToIndex = to;
    _moveCount++;
}


void Node::removeEvent(
    int index
)
{
    _lastRemoveIndex = index;
    _removeCount++;
}
}
}
