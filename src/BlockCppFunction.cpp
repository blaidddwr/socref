#include "BlockCppFunction.h"
#include <QtGui>
#include "BlockCppClass.h"
#include "Exception.h"
#include "ExceptionBlockRead.h"
#include "Global.h"
namespace Block {
namespace Cpp {
QHash<QString,int>* Function::_reverseFlagLookup {nullptr};
const QHash<int,QString> Function::_FLAG_STRINGS {
    {NoExceptFunctionFlag,"noexcept"}
    ,{ExplicitFunctionFlag,"explicit"}
    ,{StaticFunctionFlag,"static"}
    ,{ConstantFunctionFlag,"const"}
    ,{VirtualFunctionFlag,"virtual"}
    ,{OverrideFunctionFlag,"override"}
    ,{FinalFunctionFlag,"final"}
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


Function::Function(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Base(meta,parent)
    ,_icon(iconPublic())
{
}


Function::~Function(
)
{
    if (auto p = qobject_cast<Class*>(parent()))
    {
        p->updateDisplayIcon(this);
    }
}


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
    G_ASSERT(_icon);
    return *_icon;
}


QString Function::displayText(
) const
{
    QStringList ret;
    if (!_templates.isEmpty())
    {
        ret.append("template<"+_templates.join(",")+">");
    }
    appendLeftFlags(ret);
    appendReturn(ret);
    appendSignature(ret);
    appendRightSignatureFlags(ret);
    appendRightFlags(ret);
    appendAssignment(ret);
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
        updateDisplayIcon();
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
        updateDisplayIcon();
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
        updateDisplayIcon();
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
        updateDisplayIcon();
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


void Function::appendAssignment(
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


void Function::appendLeftFlags(
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


void Function::appendReturn(
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


void Function::appendRightFlags(
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


void Function::appendRightSignatureFlags(
    QStringList& words
) const
{
    if (_flags&ConstantFunctionFlag)
    {
        words.append("const");
    }
}


void Function::appendSignature(
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
    auto reverseLookup = reverseFlagLookup();
    for (const auto& flagString: value.toString().split(";"))
    {
        auto flag = reverseLookup.value(flagString,-1);
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
    auto reverseLookup = reverseFlagLookup();
    for (auto i = reverseLookup.begin();i != reverseLookup.end();i++)
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
        p->updateDisplayIcon(this);
        disconnect(p,&Base::nameChanged,this,&Function::onClassNameChanged);
    }
}


void Function::setDisplayIcon(
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


QString Function::scopeName(
) const
{
    QStringList ret;
    appendSignature(ret);
    appendRightSignatureFlags(ret);
    return ret.join("");
}


void Function::updateDisplayIcon(
)
{
    if (isAbstract())
    {
        switch (_type)
        {
        case MethodFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconAbstractPublic());
                break;
            case ProtectedAccess:
                setDisplayIcon(iconAbstractProtected());
                break;
            }
            break;
        case DestructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconAbstractDestructorPublic());
                break;
            case ProtectedAccess:
                setDisplayIcon(iconAbstractDestructorProtected());
                break;
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
                setDisplayIcon(iconVirtualPublic());
                break;
            case ProtectedAccess:
                setDisplayIcon(iconVirtualProtected());
                break;
            }
            break;
        case DestructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconVirtualDestructorPublic());
                break;
            case ProtectedAccess:
                setDisplayIcon(iconVirtualDestructorProtected());
                break;
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
            if (_flags&StaticFunctionFlag)
            {
                switch(_access)
                {
                case PublicAccess:
                    setDisplayIcon(iconStaticPublic());
                    break;
                case ProtectedAccess:
                    setDisplayIcon(iconStaticProtected());
                    break;
                case PrivateAccess:
                    setDisplayIcon(iconStaticPrivate());
                    break;
                }
            }
            else
            {
                switch(_access)
                {
                case PublicAccess:
                    setDisplayIcon(iconPublic());
                    break;
                case ProtectedAccess:
                    setDisplayIcon(iconProtected());
                    break;
                case PrivateAccess:
                    setDisplayIcon(iconPrivate());
                    break;
                }
            }
            break;
        case ConstructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconConstructorPublic());
                break;
            case ProtectedAccess:
                setDisplayIcon(iconConstructorProtected());
                break;
            case PrivateAccess:
                setDisplayIcon(iconConstructorPrivate());
                break;
            }
            break;
        case DestructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconDestructorPublic());
                break;
            case ProtectedAccess:
                setDisplayIcon(iconDestructorProtected());
                break;
            case PrivateAccess:
                setDisplayIcon(iconDestructorPrivate());
                break;
            }
            break;
        case OperatorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconOperatorPublic());
                break;
            case ProtectedAccess:
                setDisplayIcon(iconOperatorProtected());
                break;
            case PrivateAccess:
                setDisplayIcon(iconOperatorPrivate());
                break;
            }
            break;
        }
    }
    setDisplayIcon(iconInvalid());
}


const QHash<QString,int>& Function::reverseFlagLookup(
)
{
    if (!_reverseFlagLookup)
    {
        _reverseFlagLookup = new QHash<QString,int>;
        for (auto i = _FLAG_STRINGS.begin();i != _FLAG_STRINGS.end();i++)
        {
            _reverseFlagLookup->insert(i.value(),i.key());
        }
    }
    return *_reverseFlagLookup;
}


const QIcon* Function::iconAbstractDestructorProtected(
)
{
    static const QIcon ret(":/cpp/abstract_protected_destructor.svg");
    return &ret;
}


const QIcon* Function::iconAbstractDestructorPublic(
)
{
    static const QIcon ret(":/cpp/abstract_public_destructor.svg");
    return &ret;
}


const QIcon* Function::iconAbstractProtected(
)
{
    static const QIcon ret(":/cpp/abstract_protected_function.svg");
    return &ret;
}


const QIcon* Function::iconAbstractPublic(
)
{
    static const QIcon ret(":/cpp/abstract_public_function.svg");
    return &ret;
}


const QIcon* Function::iconConstructorPrivate(
)
{
    static const QIcon ret(":/cpp/private_constructor.svg");
    return &ret;
}


const QIcon* Function::iconConstructorProtected(
)
{
    static const QIcon ret(":/cpp/protected_constructor.svg");
    return &ret;
}


const QIcon* Function::iconConstructorPublic(
)
{
    static const QIcon ret(":/cpp/public_constructor.svg");
    return &ret;
}


const QIcon* Function::iconDestructorPrivate(
)
{
    static const QIcon ret(":/cpp/private_desctructor.svg");
    return &ret;
}


const QIcon* Function::iconDestructorProtected(
)
{
    static const QIcon ret(":/cpp/protected_desctructor.svg");
    return &ret;
}


const QIcon* Function::iconDestructorPublic(
)
{
    static const QIcon ret(":/cpp/public_desctructor.svg");
    return &ret;
}


const QIcon* Function::iconInvalid(
)
{
    static const QIcon ret(":/cpp/invalid_function.svg");
    return &ret;
}


const QIcon* Function::iconOperatorPrivate(
)
{
    static const QIcon ret(":/cpp/private_operator.svg");
    return &ret;
}


const QIcon* Function::iconOperatorProtected(
)
{
    static const QIcon ret(":/cpp/protected_operator.svg");
    return &ret;
}


const QIcon* Function::iconOperatorPublic(
)
{
    static const QIcon ret(":/cpp/public_operator.svg");
    return &ret;
}


const QIcon* Function::iconPrivate(
)
{
    static const QIcon ret(":/cpp/private_function.svg");
    return &ret;
}


const QIcon* Function::iconProtected(
)
{
    static const QIcon ret(":/cpp/protected_function.svg");
    return &ret;
}


const QIcon* Function::iconPublic(
)
{
    static const QIcon ret(":/cpp/public_function.svg");
    return &ret;
}


const QIcon* Function::iconStaticPrivate(
)
{
    static const QIcon ret(":/cpp/static_private_function.svg");
    return &ret;
}


const QIcon* Function::iconStaticProtected(
)
{
    static const QIcon ret(":/cpp/static_protected_function.svg");
    return &ret;
}


const QIcon* Function::iconStaticPublic(
)
{
    static const QIcon ret(":/cpp/static_public_function.svg");
    return &ret;
}


const QIcon* Function::iconVirtualDestructorProtected(
)
{
    static const QIcon ret(":/cpp/virtual_protected_destructor.svg");
    return &ret;
}


const QIcon* Function::iconVirtualDestructorPublic(
)
{
    static const QIcon ret(":/cpp/virtual_public_destructor.svg");
    return &ret;
}


const QIcon* Function::iconVirtualProtected(
)
{
    static const QIcon ret(":/cpp/virtual_protected_function.svg");
    return &ret;
}


const QIcon* Function::iconVirtualPublic(
)
{
    static const QIcon ret(":/cpp/virtual_public_function.svg");
    return &ret;
}
}
}
