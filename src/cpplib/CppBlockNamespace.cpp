#include "CppBlockNamespace.h"
#include <QtGui>
#include "CppBlockWidgetNamespaceEdit.h"
namespace Cpp {
namespace Block {


Namespace::Namespace(
    Model::Meta::Block* meta
    ,QObject* parent
):
    AbstractBlock(meta,parent)
    ,_name("namespace")
{
}


AbstractBlockWidget* Namespace::createWidget(
) const
{
    return new BlockWidget::NamespaceEdit(this);
}


const QString& Namespace::description(
) const
{
    return _description;
}


QIcon Namespace::displayIcon(
) const
{
    return QIcon(":/cpp/namespace.svg");
}


QString Namespace::displayText(
) const
{
    return _name;
}


QString Namespace::fileName(
) const
{
    return qobject_cast<AbstractBlock*>(parent())? _name : rootFileName();
}


QString Namespace::label(
) const
{
    if (_name.isNull())
    {
        return "::";
    }
    QStringList ret {_name};
    auto p = qobject_cast<Namespace*>(parent());
    while (p)
    {
        if (!p->_name.isNull())
        {
            ret.append(p->_name);
        }
        p = qobject_cast<Namespace*>(p->parent());
    }
    std::reverse(ret.begin(),ret.end());
    return ret.join("::");
}


void Namespace::loadFromMap(
    const QMap<QString,QVariant>& map
    ,int version
)
{
    Q_UNUSED(version);
    setName(map.value(nameKey()).toString());
    setDescription(map.value(descriptionKey()).toString());
}


const QString& Namespace::name(
) const
{
    return _name;
}


QMap<QString,QVariant> Namespace::saveToMap(
) const
{
    QMap<QString,QVariant> ret;
    if (!_name.isEmpty())
    {
        ret.insert(nameKey(),_name);
    }
    if (!_description.isEmpty())
    {
        ret.insert(descriptionKey(),_description);
    }
    return ret;
}


void Namespace::setDescription(
    const QString& value
)
{
    if (_description != value)
    {
        _description = value;
        emit descriptionChanged(value);
    }
}


void Namespace::setName(
    const QString& value
)
{
    if (_name != value)
    {
        _name = value;
        onNameChanged(value);
        emit nameChanged(value);
    }
}


void Namespace::setState(
    const QHash<QString,QVariant>& state
)
{
    setName(state.value(nameKey()).toString());
    setDescription(state.value(descriptionKey()).toString());
}


QHash<QString,QVariant> Namespace::state(
) const
{
    return
    {
        {nameKey(),_name}
        ,{descriptionKey(),_description}
    };
}


Namespace::Namespace(
    const QString& name
    ,Model::Meta::Block* meta
    ,QObject* parent
):
    AbstractBlock(meta,parent)
    ,_name(name)
{
}


AbstractBlock* Namespace::create(
    QObject* parent
) const
{
    return new Namespace(meta(),parent);
}


void Namespace::onNameChanged(
    const QString& value
)
{
    emit displayTextChanged(value);
}


const char* Namespace::descriptionKey(
)
{
    return "description";
}


const char* Namespace::nameKey(
)
{
    return "name";
}
}
}
