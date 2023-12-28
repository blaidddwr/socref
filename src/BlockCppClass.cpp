#include "BlockCppClass.h"
#include <QtGui>
#include "Global.h"
namespace Block {
namespace Cpp {


Class::Class(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Property("class",icon(),meta,parent)
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
}
}
