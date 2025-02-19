#include "CppBlockProperty.h"
#include <QtGui>
#include "CppBlockFunction.h"
#include "CppBlockWidgetPropertyEdit.h"
namespace Cpp {
namespace Block {
bool Property::_iconsInitialized {false};
const QIcon* Property::_abstractIcon {nullptr};
const QIcon* Property::_regularIcon {nullptr};
const QIcon* Property::_virtualIcon {nullptr};


Property::Property(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("property",meta,parent)
    ,_icon(_regularIcon)
{
    Q_ASSERT(_iconsInitialized);
}


Property::~Property(
)
{
    if (auto p = qobject_cast<Property*>(parent()))
    {
        auto index = p->indexOf(this);
        Q_ASSERT(index != -1);
        p->take(index);
    }
}


AbstractBlockWidget* Property::createWidget(
) const
{
    return new BlockWidget::PropertyEdit(this);
}


QIcon Property::displayIcon(
) const
{
    Q_ASSERT(_icon);
    return *_icon;
}


void Property::initializeIcons(
)
{
    if (!_iconsInitialized)
    {
        _abstractIcon = new QIcon(":/cpp/abstract_property.svg");
        _regularIcon = new QIcon(":/cpp/property.svg");
        _virtualIcon = new QIcon(":/cpp/virtual_property.svg");
        _iconsInitialized = true;
    }
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
        setDisplayIcon(_abstractIcon);

    }
    else if (isVirtual())
    {
        setDisplayIcon(_virtualIcon);
    }
    else
    {
        setDisplayIcon(_regularIcon);
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
    Q_ASSERT(icon);
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


AbstractBlock* Property::create(
    QObject* parent
) const
{
    return new Property(meta(),parent);
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
    Q_ASSERT(pointer);
    _icon = pointer;
    emit displayIconChanged(*_icon);
}
}
}
