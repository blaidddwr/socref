#include "CppBlockClass.h"
#include <QtGui>
#include "CppBlockWidgetClassEdit.h"
#include "Global.h"
#define PARENTS "parents"
#define TEMPLATES "templates"
namespace Cpp {
namespace Block {
bool Class::_iconsInitialized {false};
const QIcon* Class::_abstractIcon {nullptr};
const QIcon* Class::_regularIcon {nullptr};
const QIcon* Class::_virtualIcon {nullptr};


Class::Class(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Property("class",_regularIcon,meta,parent)
{
    Q_ASSERT(_iconsInitialized);
}


AbstractBlockWidget* Class::createWidget(
) const
{
    return new BlockWidget::ClassEdit(this);
}


QString Class::displayText(
) const
{
    auto ret = Base::displayText();
    if (!_parents.isEmpty())
    {
        ret += " : "+_parents.join(", ");
    }
    if (!_templates.isEmpty())
    {
        ret += " -> template<"+_templates.join(",")+">";
    }
    return ret;
}


void Class::initializeIcons(
)
{
    if (!_iconsInitialized)
    {
        _abstractIcon = new QIcon(":/cpp/abstract_class.svg");
        _regularIcon = new QIcon(":/cpp/class.svg");
        _virtualIcon = new QIcon(":/cpp/virtual_class.svg");
        _iconsInitialized = true;
    }
}


void Class::loadFromMap(
    const QMap<QString,QVariant>& map
    ,int version
)
{
    Base::loadFromMap(map,version);
    if (version == Socref_Legacy)
    {
        _parents = map.value(PARENTS).toString().split('\n',Qt::SkipEmptyParts);
        auto str = map.value("template").toString();
        str = str.replace("template","").replace("<","").replace(">","");
        _templates = str.split(',',Qt::SkipEmptyParts);
        for (auto& t: _templates)
        {
            t = t.trimmed();
        }
    }
    else
    {
        _parents = map.value(PARENTS).toString().split(';',Qt::SkipEmptyParts);
        _templates = map.value(TEMPLATES).toString().split(';',Qt::SkipEmptyParts);
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
        ret.insert(PARENTS,_parents.join(';'));
    }
    if (!_templates.isEmpty())
    {
        ret.insert(TEMPLATES,_templates.join(';'));
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
        emit displayTextChanged(displayText());
    }
}


void Class::setState(
    const QHash<QString,QVariant>& state
)
{
    Base::setState(state);
    setParents(state.value(PARENTS).toStringList());
    setTemplates(state.value(TEMPLATES).toStringList());
}


void Class::setTemplates(
    const QStringList& value
)
{
    if (_templates != value)
    {
        _templates = value;
        emit templatesChanged(value);
        emit displayTextChanged(displayText());
    }
}


QHash<QString,QVariant> Class::state(
) const
{
    auto ret = Base::state();
    ret.insert(PARENTS,_parents);
    ret.insert(TEMPLATES,_templates);
    return ret;
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
}


AbstractBlock* Class::create(
    QObject* parent
) const
{
    return new Class(meta(),parent);
}
}
}
