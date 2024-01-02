#include "BlockCppQtProperty.h"
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


const QString& Property::writeMethod(
) const
{
    return _writeMethod;
}
}
}
