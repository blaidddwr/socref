#include "BlockCppFunction.h"
#include <QtGui>
#include "BlockCppClass.h"
#include "Exception.h"
#include "ExceptionBlockRead.h"
#include "Global.h"
namespace Block {
namespace Cpp {
const QHash<QString,int> Function::_REVERSE_FLAG_LOOKUP {
    {"noexcept",NoExceptFunctionFlag}
    ,{"explicit",NoExceptFunctionFlag}
    ,{"static",NoExceptFunctionFlag}
    ,{"const",NoExceptFunctionFlag}
    ,{"virtual",NoExceptFunctionFlag}
    ,{"override",NoExceptFunctionFlag}
    ,{"final",NoExceptFunctionFlag}
};
const QHash<int,QString> Function::_FLAG_STRINGS {
    {NoExceptFunctionFlag,"noexcept"}
    ,{NoExceptFunctionFlag,"explicit"}
    ,{NoExceptFunctionFlag,"static"}
    ,{NoExceptFunctionFlag,"const"}
    ,{NoExceptFunctionFlag,"virtual"}
    ,{NoExceptFunctionFlag,"override"}
    ,{NoExceptFunctionFlag,"final"}
};
const QStringList Function::_ACCESS_STRINGS = {
    "public"
    ,"protected"
    ,"private"
};
const QStringList Function::_ASSIGNMENT_STRINGS = {
    "none"
    ,"default"
    ,"delete"
    ,"abstract"
};
const QStringList Function::_TYPE_STRINGS = {
    "regular"
    ,"method"
    ,"constructor"
    ,"destructor"
    ,"operator"
};


int Function::access(
) const
{
    return _access;
}


QString Function::accessString(
) const
{
    return _ACCESS_STRINGS.at(_access);
}


int Function::assignment(
) const
{
    return _assignment;
}


QString Function::assignmentString(
) const
{
    return _ASSIGNMENT_STRINGS.at(_assignment);
}


Widget::Block::Abstract* Function::createWidget(
    QObject* parent
) const
{
    Q_UNUSED(parent);
    return nullptr;//TODO
}


QIcon Function::displayIcon(
) const
{
    if (isAbstract())
    {
        switch (_type)
        {
        case MethodFunctionType:
            switch(_access)
            {
            case PublicAccess:
                return QIcon(":/cpp/abstract_public_function.svg");
            case ProtectedAccess:
                return QIcon(":/cpp/abstract_protected_function.svg");
            }
            break;
        case DestructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                return QIcon(":/cpp/abstract_public_destructor.svg");
            case ProtectedAccess:
                return QIcon(":/cpp/abstract_protected_destructor.svg");
            }
            break;
        }
    }
    else if (isVirtual())
    {
        switch (_type)
        {
        case MethodFunctionType:
            switch(_access)
            {
            case PublicAccess:
                return QIcon(":/cpp/virtual_public_function.svg");
            case ProtectedAccess:
                return QIcon(":/cpp/virtual_protected_function.svg");
            }
            break;
        case DestructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                return QIcon(":/cpp/virtual_public_destructor.svg");
            case ProtectedAccess:
                return QIcon(":/cpp/virtual_protected_destructor.svg");
            }
            break;
        }
    }
    else
    {
        switch (_type)
        {
        case RegularFunctionType:
        case MethodFunctionType:
            switch(_access)
            {
            case PublicAccess:
                return QIcon(":/cpp/public_function.svg");
            case ProtectedAccess:
                return QIcon(":/cpp/protected_function.svg");
            case PrivateAccess:
                return QIcon(":/cpp/private_function.svg");
            }
            break;
        case ConstructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                return QIcon(":/cpp/public_constructor.svg");
            case ProtectedAccess:
                return QIcon(":/cpp/protected_constructor.svg");
            case PrivateAccess:
                return QIcon(":/cpp/private_constructor.svg");
            }
            break;
        case DestructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                return QIcon(":/cpp/public_destructor.svg");
            case ProtectedAccess:
                return QIcon(":/cpp/protected_destructor.svg");
            case PrivateAccess:
                return QIcon(":/cpp/private_destructor.svg");
            }
            break;
        case OperatorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                return QIcon(":/cpp/public_operator.svg");
            case ProtectedAccess:
                return QIcon(":/cpp/protected_operator.svg");
            case PrivateAccess:
                return QIcon(":/cpp/private_operator.svg");
            }
            break;
        }
    }
    return QIcon(":/cpp/invalid_function.svg");
}


QString Function::displayText(
) const
{
    QStringList ret;
    if (!_templates.isEmpty())
    {
        ret.append("template<"+_templates.join(",")+">");
    }
    addLeftFlags(ret);
    addReturn(ret);
    addSignature(ret);
    addRightSignatureFlags(ret);
    addRightFlags(ret);
    addAssignment(ret);
    return ret.join(" ");
}


QStringList Function::flagStrings(
) const
{
    QStringList ret;
    for (auto i = _FLAG_STRINGS.begin();i != _FLAG_STRINGS.end();i++)
    {
        if (_flags&i.key())
        {
            ret.append(i.value());
        }
    }
    return ret;
}


int Function::flags(
) const
{
    return _flags;
}


bool Function::isAbstract(
) const
{
    return _assignment == AbstractFunctionAssignment;
}


bool Function::isVirtual(
) const
{
    return _flags&VirtualFunctionFlag;
}


void Function::loadFromMap(
    const QMap<QString,QVariant>& map
    ,int version
)
{
    Base::loadFromMap(map,version);
    _returnType = map.value("returnType").toString();
    _returnDescription = map.value("returnDescription").toString();
    _type = loadType(map.value("type"),version);
    _access = loadAccess(map.value("access"),version);
    if (version == Socref_Legacy)
    {
        _flags = loadFlagsLegacy(map);
        _assignment = loadAssignmentLegacy(map);
    }
    else
    {
        _flags = loadFlags(map.value("flags"),version);
        _assignment = loadAssignment(map.value("assignment"),version);
    }
}


const QString& Function::returnDescription(
) const
{
    return _returnDescription;
}


const QString& Function::returnType(
) const
{
    return _returnType;
}


QMap<QString,QVariant> Function::saveToMap(
) const
{
    auto ret = Base::saveToMap();
    ret.insert("type",typeString());
    ret.insert("access",accessString());
    ret.insert("returnType",_returnType);
    if (!_returnDescription.isEmpty())
    {
        ret.insert("returnDescription",_returnDescription);
    }
    if (!_templates.isEmpty())
    {
        ret.insert("template",_templates.join(';'));
    }
    auto flags = flagStrings();
    if (!flags.isEmpty())
    {
        ret.insert("flags",flags.join(";"));
    }
    ret.insert("assignment",assignmentString());
    return ret;
}


void Function::setAccess(
    int value
)
{
    if (_access != value)
    {
        _access = value;
        emit accessChanged(value);
        emit displayIconChanged(displayIcon());
    }
}


void Function::setAssignment(
    int value
)
{
    if (_assignment != value)
    {
        _assignment = value;
        emit assignmentChanged(value);
        emit displayTextChanged(displayText());
        emit displayIconChanged(displayIcon());
        if (auto p = qobject_cast<Class*>(parent()))
        {
            p->updateDisplayIcon();
        }
    }
}


void Function::setFlags(
    int value
)
{
    if (_flags != value)
    {
        _flags = value;
        emit flagsChanged(value);
        emit displayTextChanged(displayText());
        emit displayIconChanged(displayIcon());
        if (auto p = qobject_cast<Class*>(parent()))
        {
            p->updateDisplayIcon();
        }
    }
}


void Function::setReturnDescription(
    const QString& value
)
{
    if (_returnDescription != value)
    {
        _returnDescription = value;
        emit returnDescriptionChanged(value);
    }
}


void Function::setReturnType(
    const QString& value
)
{
    if (_returnType != value)
    {
        _returnType = value;
        emit returnTypeChanged(value);
        emit displayTextChanged(displayText());
    }
}


void Function::setTemplates(
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


void Function::setType(
    int value
)
{
    if (_type != value)
    {
        _type = value;
        emit typeChanged(value);
        emit displayTextChanged(displayText());
        emit displayIconChanged(displayIcon());
    }
}


const QStringList& Function::templates(
) const
{
    return _templates;
}


int Function::type(
) const
{
    return _type;
}


QString Function::typeString(
) const
{
    return _TYPE_STRINGS.at(_type);
}


QStringList Function::arguments(
    bool onlyTypes
) const
{
    Q_UNUSED(onlyTypes);
    return {};//TODO: need variable block to do this
}


void Function::addAssignment(
    QStringList& words
) const
{
    switch (_assignment)
    {
    case DefaultFunctionAssignment:
        words.append("=");
        words.append("default");
        break;
    case DeleteFunctionAssignment:
        words.append("=");
        words.append("delete");
        break;
    case AbstractFunctionAssignment:
        words.append("=");
        words.append("0");
        break;
    }
}


void Function::addEvent(
)
{
    if (auto p = qobject_cast<Class*>(parent()))
    {
        if (_type == RegularFunctionType)
        {
            setType(MethodFunctionType);
        }
        connect(p,&Base::nameChanged,this,&Function::onClassNameChanged);
        p->updateDisplayIcon();
    }
    else
    {
        setType(RegularFunctionType);
        setAccess(PublicAccess);
        setFlags(flags()&NoExceptFunctionFlag);
        setAssignment(NoFunctionAssignment);
    }
}


void Function::addLeftFlags(
    QStringList& words
) const
{
    if (_flags&StaticFunctionFlag)
    {
        words.append("static");
    }
    if (_flags&VirtualFunctionFlag)
    {
        words.append("virtual");
    }
    if (_flags&ExplicitFunctionFlag)
    {
        words.append("explicit");
    }
}


void Function::addReturn(
    QStringList& words
) const
{
    switch (_type)
    {
    case RegularFunctionType:
    case MethodFunctionType:
    case OperatorFunctionType:
        words.append(_returnType);
        break;
    }
}


void Function::addRightFlags(
    QStringList& words
) const
{
    if (_flags&NoExceptFunctionFlag)
    {
        words.append("noexcept");
    }
    if (_flags&OverrideFunctionFlag)
    {
        words.append("override");
    }
    if (_flags&FinalFunctionFlag)
    {
        words.append("final");
    }
}


void Function::addRightSignatureFlags(
    QStringList& words
) const
{
    if (_flags&ConstantFunctionFlag)
    {
        words.append("const");
    }
}


void Function::addSignature(
    QStringList& words
) const
{
    switch (_type)
    {
    case RegularFunctionType:
    case MethodFunctionType:
        words.append(name()+"("+arguments(true).join(",")+")");
        break;
    case ConstructorFunctionType:
    case DestructorFunctionType:
    {
        auto parentBlock = qobject_cast<Class*>(parent());
        G_ASSERT(parentBlock);
        if (_type == DestructorFunctionType)
        {
            words.append("~"+parentBlock->name()+"("+arguments(true).join(",")+")");
        }
        words.append(parentBlock->name()+"("+arguments(true).join(",")+")");
        break;
    }
    case OperatorFunctionType:
        words.append("operator"+name()+"("+arguments(true).join(",")+")");
        break;
    }
}


int Function::loadAccess(
    const QVariant& value
    ,int version
)
{
    auto accessString = value.toString();
    if (version == Socref_Legacy)
    {
        accessString = accessString.toLower();
    }
    auto ret = _ACCESS_STRINGS.indexOf(accessString);
    if (ret == -1)
    {
        throw Exception::Block::Read(tr("Unknown C++ access %1.").arg(accessString));
    }
    return ret;
}


int Function::loadAssignment(
    const QVariant& value
    ,int version
)
{
    Q_UNUSED(version);
    auto assignmentString = value.toString();
    auto ret = _ASSIGNMENT_STRINGS.indexOf(assignmentString);
    if (ret == -1)
    {
        throw Exception::Block::Read(
            tr("Unknown C++ function assignment %1").arg(assignmentString)
        );
    }
    return ret;
}


int Function::loadAssignmentLegacy(
    const QMap<QString,QVariant>& map
)
{
    if (
        map.contains("abstract")
        && map.value("abstract").toBool()
    )
    {
        return AbstractFunctionAssignment;
    }
    if (
        map.contains("default")
        && map.value("default").toBool()
        )
    {
        return DefaultFunctionAssignment;
    }
    if (
        map.contains("deleted")
        && map.value("deleted").toBool()
        )
    {
        return DeleteFunctionAssignment;
    }
    return NoFunctionAssignment;
}


int Function::loadFlags(
    const QVariant& value
    ,int version
)
{
    Q_UNUSED(version);
    int ret = 0;
    for (const auto& flagString: value.toString().split(";"))
    {
        auto flag = _REVERSE_FLAG_LOOKUP.value(flagString,-1);
        if (flag != -1)
        {
            ret |= flag;
        }
    }
    return ret;
}


int Function::loadFlagsLegacy(
    const QMap<QString,QVariant>& map
)
{
    int ret = 0;
    for (auto i = _REVERSE_FLAG_LOOKUP.begin();i != _REVERSE_FLAG_LOOKUP.end();i++)
    {
        if (
            map.contains(i.key())
            && map.value(i.key()).toBool()
        )
        {
            ret |= i.value();
        }
    }
    return ret;
}


int Function::loadType(
    const QVariant& value
    ,int version
)
{
    if (version == Socref_Legacy)
    {
        if (name() == "^")
        {
            setName("");
            return ConstructorFunctionType;
        }
        else if (name() == "~^")
        {
            setName("");
            return DestructorFunctionType;
        }
        else if (name().startsWith("operator"))
        {
            setName(name().mid(8));
            return OperatorFunctionType;
        }
        else
        {
            return RegularFunctionType;
        }
    }
    else
    {
        auto typeString = value.toString();
        auto ret = _TYPE_STRINGS.indexOf(typeString);
        if (ret == -1)
        {
            throw Exception::Block::Read(tr("Unknown C++ function type %1.").arg(typeString));
        }
        return ret;
    }
}


void Function::onClassNameChanged(
    const QString& value
)
{
    Q_UNUSED(value);
    if (
        _type == ConstructorFunctionType
        || _type == DestructorFunctionType
    )
    {
        emit displayTextChanged(displayText());
    }
}


void Function::removeEvent(
)
{
    if (auto p = qobject_cast<Class*>(parent()))
    {
        disconnect(p,&Base::nameChanged,this,&Function::onClassNameChanged);
    }
}


QString Function::scopeName(
) const
{
    QStringList ret;
    addSignature(ret);
    addRightSignatureFlags(ret);
    return ret.join("");
}
}
}
