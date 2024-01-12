#include "BlockCppQtProperty.h"
#include <QtCore>
namespace Block {
namespace CppQt {


const QString& Property::notifySignal(
) const
{
    return _notifySignal;
}


const QString& Property::readMethod(
) const
{
    return _readMethod;
}


void Property::setNotifySignal(
    const QString& value
)
{
    if (_notifySignal != value)
    {
        _notifySignal = value;
        emit notifySignalChanged(value);
    }
}


void Property::setReadMethod(
    const QString& value
)
{
    if (_readMethod != value)
    {
        _readMethod = value;
        emit readMethodChanged(value);
    }
}


void Property::setState(
    const QHash<QString,QVariant>& state
)
{
    Cpp::Property::setState(state);
    setReadMethod(state.value("readMethod").toString());
    setWriteMethod(state.value("writeMethod").toString());
    setNotifySignal(state.value("notifySignal").toString());
}


void Property::setWriteMethod(
    const QString& value
)
{
    if (_writeMethod != value)
    {
        _writeMethod = value;
        emit writeMethodChanged(value);
    }
}


QHash<QString,QVariant> Property::state(
) const
{
    auto ret = Cpp::Property::state();
    ret.insert("readMethod",_readMethod);
    ret.insert("writeMethod",_writeMethod);
    ret.insert("notifySignal",_notifySignal);
    return ret;
}


const QString& Property::writeMethod(
) const
{
    return _writeMethod;
}
}
}
