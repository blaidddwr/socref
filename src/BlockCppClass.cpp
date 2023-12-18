#include "BlockCppClass.h"
#include <QtGui>
#include "BlockCppFunction.h"
#include "Exception.h"
#include "Global.h"
namespace Block {
namespace Cpp {


Class::Class(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base("class",meta,parent)
    ,_icon(icon())
{
}


Widget::Block::Abstract* Class::createWidget(
    QObject* parent
) const
{
    //TODO
    Q_UNUSED(parent);
    return nullptr;
}


QIcon Class::displayIcon(
) const
{
    G_ASSERT(_icon);
    return *_icon;
}


bool Class::isAbstract(
    const Block::Abstract* ignore
) const
{
    for (int i = 0;i < size();i++)
    {
        if (auto f = qobject_cast<Function*>(get(i)))
        {
            if (
                f != ignore
                && f->isAbstract()
            )
            {
                return true;
            }
        }
    }
    return false;
}


bool Class::isVirtual(
    const Block::Abstract* ignore
) const
{
    for (int i = 0;i < size();i++)
    {
        if (auto f = qobject_cast<Function*>(get(i)))
        {
            if (
                f != ignore
                && f->isVirtual()
            )
            {
                return true;
            }
        }
    }
    return false;
}


void Class::loadFromMap(
    const QMap<QString,QVariant>& map
    ,int version
)
{
    Base::loadFromMap(map,version);
    if (version == Socref_Legacy)
    {
        _parents = map.value("parents").toString().split('\n',Qt::SkipEmptyParts);
        auto str = map.value("template").toString();
        str = str.replace("template","").replace("<","").replace(">","");
        _templates = str.split(',');
        for (auto& t: _templates)
        {
            t = t.trimmed();
        }
    }
    else
    {
        _parents = map.value("parents").toString().split(';');
        _templates = map.value("templates").toString().split(';');
    }
}


const QStringList& Class::parents(
) const
{
    return _parents;
}


QMap<QString,QVariant> Class::saveToMap(
) const
{
    auto ret = Base::saveToMap();
    if (!_parents.isEmpty())
    {
        ret.insert("parents",_parents.join(';'));
    }
    if (!_templates.isEmpty())
    {
        ret.insert("template",_templates.join(';'));
    }
    return ret;
}


void Class::setParents(
    const QStringList& value
)
{
    if (_parents != value)
    {
        _parents = value;
        emit parentsChanged(value);
    }
}


void Class::setTemplates(
    const QStringList& value
)
{
    if (_templates != value)
    {
        _templates = value;
        emit templatesChanged(value);
    }
}


const QStringList& Class::templates(
) const
{
    return _templates;
}


void Class::updateDisplayIcon(
    const Block::Abstract* ignore
)
{
    if (isAbstract(ignore))
    {
        setDisplayIcon(iconAbstract());

    }
    else if (isVirtual(ignore))
    {
        setDisplayIcon(iconVirtual());
    }
    else
    {
        setDisplayIcon(icon());
    }
}


const QIcon* Class::icon(
)
{
    static const QIcon ret(":/cpp/class.svg");
    return &ret;
}


const QIcon* Class::iconAbstract(
)
{
    static const QIcon ret(":/cpp/abstract_class.svg");
    return &ret;
}


const QIcon* Class::iconVirtual(
)
{
    static const QIcon ret(":/cpp/virtual_class.svg");
    return &ret;
}


void Class::setDisplayIcon(
    const QIcon* pointer
)
{
    G_ASSERT(pointer);
    if (_icon != pointer)
    {
        _icon = pointer;
        emit displayIconChanged(*pointer);
    }
}
}
}
