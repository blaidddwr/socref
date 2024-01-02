#include "BlockCppProperty.h"
#include <QtGui>
#include "BlockCppFunction.h"
#include "Exception.h"
namespace Block {
namespace Cpp {


Property::Property(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("property",meta,parent)
    ,_icon(icon())
{
}


Property::~Property(
)
{
    if (auto p = qobject_cast<Property*>(parent()))
    {
        auto index = p->indexOf(this);
        G_ASSERT(index != -1);
        p->take(index);
    }
}


Widget::Block::Abstract* Property::createWidget(
    QObject* parent
) const
{
    Q_UNUSED(parent);
    return nullptr;//TODO
}


QIcon Property::displayIcon(
) const
{
    G_ASSERT(_icon);
    return *_icon;
}


bool Property::isAbstract(
) const
{
    for (int i = 0;i < size();i++)
    {
        if (auto f = qobject_cast<Function*>(get(i)))
        {
            if (f->isAbstract())
            {
                return true;
            }
        }
        else if (auto p = qobject_cast<Property*>(get(i)))
        {
            return p->isAbstract();
        }
    }
    return false;
}


bool Property::isVirtual(
) const
{
    for (int i = 0;i < size();i++)
    {
        if (auto f = qobject_cast<Function*>(get(i)))
        {
            if (f->isVirtual())
            {
                return true;
            }
        }
        else if (auto p = qobject_cast<Property*>(get(i)))
        {
            return p->isVirtual();
        }
    }
    return false;
}


void Property::updateDisplayIcon(
)
{
    if (isAbstract())
    {
        setDisplayIcon(iconAbstract());

    }
    else if (isVirtual())
    {
        setDisplayIcon(iconVirtual());
    }
    else
    {
        setDisplayIcon(icon());
    }
    if (auto p = qobject_cast<Property*>(parent()))
    {
        p->updateDisplayIcon();
    }
}


Property::Property(
    const QString& name
    ,const QIcon* icon
    ,Model::Meta::Block* meta
    ,QObject* parent
):
    Base(name,meta,parent)
    ,_icon(icon)
{
    G_ASSERT(icon);
}


void Property::addEvent(
    int index
)
{
    Q_UNUSED(index);
    if (auto p = qobject_cast<Property*>(parent()))
    {
        p->updateDisplayIcon();
    }
}


void Property::removeEvent(
    int index
)
{
    Q_UNUSED(index);
    if (auto p = qobject_cast<Property*>(parent()))
    {
        p->updateDisplayIcon();
    }
}


void Property::setDisplayIcon(
    const QIcon* pointer
)
{
    G_ASSERT(pointer);
    _icon = pointer;
    emit displayIconChanged(*_icon);
}


const QIcon* Property::icon(
)
{
    static const QIcon ret(":/cpp/property.svg");
    return &ret;
}


const QIcon* Property::iconAbstract(
)
{
    static const QIcon ret(":/cpp/abstract_property.svg");
    return &ret;
}


const QIcon* Property::iconVirtual(
)
{
    static const QIcon ret(":/cpp/virtual_property.svg");
    return &ret;
}
}
}
