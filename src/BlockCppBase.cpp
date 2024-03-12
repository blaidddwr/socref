#include "BlockCppBase.h"
#include <QtCore>
namespace Block {
namespace Cpp {


QString Base::displayText(
) const
{
    return _name;
}


const QString& Base::description(
) const
{
    return _description;
}


QString Base::fileName(
) const
{
    return qobject_cast<Abstract*>(parent())? _name : "ROOT";
}


const QString& Base::name(
) const
{
    return _name;
}


void Base::loadFromMap(
    const QMap<QString,QVariant>& map
    ,int version
)
{
    Q_UNUSED(version);
    setName(map.value(nameKey()).toString());
    setDescription(map.value(descriptionKey()).toString());
}


QMap<QString,QVariant> Base::saveToMap(
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


void Base::setDescription(
    const QString& value
)
{
    if (_description != value)
    {
        _description = value;
        emit descriptionChanged(value);
    }
}


void Base::setName(
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


void Base::setState(
    const QHash<QString,QVariant>& state
)
{
    setName(state.value(nameKey()).toString());
    setDescription(state.value(descriptionKey()).toString());
}


QHash<QString,QVariant> Base::state(
) const
{
    return {
        {nameKey(),_name}
        ,{descriptionKey(),_description}
    };
}


Base::Base(
    const QString& name
    ,Model::Meta::Block* meta
    ,QObject* parent
):
    Abstract(meta,parent)
    ,_name(name)
{
}


void Base::onNameChanged(
    const QString& value
)
{
    emit displayTextChanged(value);
}


const char* Base::descriptionKey(
)
{
    return "description";
}


const char* Base::nameKey(
)
{
    return "name";
}
}
}
