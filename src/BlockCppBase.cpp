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
    _name = map.value("name").toString();
    _description = map.value("description").toString();
}


QMap<QString,QVariant> Base::saveToMap(
) const
{
    QMap<QString,QVariant> ret;
    if (!_name.isEmpty())
    {
        ret.insert("name",_name);
    }
    if (!_description.isEmpty())
    {
        ret.insert("description",_description);
    }
    return ret;
}


QString Base::scope(
) const
{
    QStringList scopeNames {scopeName()};
    auto block = qobject_cast<Base*>(parent());
    if (!block)
    {
        return "ROOT";
    }
    while (block)
    {
        auto scopeName = block->scopeName();
        if (!scopeName.isNull())
        {
            scopeNames.prepend(scopeName);
        }
        block = qobject_cast<Base*>(block->parent());
    }
    return scopeNames.join("::");
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


void Base::onNameChanged(
    const QString& value
)
{
    emit displayTextChanged(value);
}


QString Base::scopeName(
) const
{
    if (!qobject_cast<Base*>(parent()))
    {
        return QString();
    }
    return _name;
}
}
}
