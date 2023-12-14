#include "BlockCppClass.h"
#include <QtGui>
#include "BlockCppFunction.h"
#include "Global.h"
namespace Block {
namespace Cpp {


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
    if (isAbstract())
    {
        return QIcon(":/cpp/abstract_class.svg");

    }
    else if (isVirtual())
    {
        return QIcon(":/cpp/virtual_class.svg");
    }
    else
    {
        return QIcon(":/cpp/class.svg");
    }
}


bool Class::isAbstract(
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
    }
    return false;
}


bool Class::isVirtual(
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
)
{
    emit displayIconChanged(displayIcon());
}
}
}
