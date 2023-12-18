#include "BlockCppFunction.h"
#include <QtGui>
#include "BlockCppClass.h"
#include "Exception.h"
#include "ExceptionBlockLogical.h"
#include "ExceptionBlockRead.h"
#include "Global.h"
namespace Block {
namespace Cpp {
QHash<QString,int>* Function::_reverseFlagLookup {nullptr};
const QMap<int,QString> Function::_FLAG_STRINGS {
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
    Base("function",meta,parent)
    ,_displayText("function() -> void")
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
    return _displayText;
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


bool Function::isConstant(
) const
{
    return _flags&ConstantFunctionFlag;
}


bool Function::isConstructor(
) const
{
    return _type == ConstructorFunctionType;
}


bool Function::isDefault(
) const
{
    return _assignment == DefaultFunctionAssignment;
}


bool Function::isDeleted(
) const
{
    return _assignment == DeleteFunctionAssignment;
}


bool Function::isDestructor(
) const
{
    return _type == DestructorFunctionType;
}


bool Function::isExplicit(
) const
{
    return _flags&ExplicitFunctionFlag;
}


bool Function::isFinal(
) const
{
    return _flags&FinalFunctionFlag;
}


bool Function::isMethod(
) const
{
    return _type == MethodFunctionType;
}


bool Function::isNoExcept(
) const
{
    return _flags&NoExceptFunctionFlag;
}


bool Function::isOperator(
) const
{
    return _type == OperatorFunctionType;
}


bool Function::isOverride(
) const
{
    return _flags&OverrideFunctionFlag;
}


bool Function::isPrivate(
) const
{
    return _access == PrivateAccess;
}


bool Function::isProtected(
) const
{
    return _access == ProtectedAccess;
}


bool Function::isPublic(
) const
{
    return _access == PublicAccess;
}


bool Function::isStatic(
) const
{
    return _flags&StaticFunctionFlag;
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
    updateDisplayIcon();
    updateDisplayText();
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


void Function::set(
    const QString& name
    ,const QString& returnType
    ,int type
    ,int access
    ,int assignment
    ,int flags
)
{
    if (
        Base::name() != name
        || _returnType != returnType
        || _type != type
        || _access != access
        || _assignment != assignment
        || _flags != flags
    )
    {
        auto oldName = Base::name();
        auto oldReturnType = _returnType;
        auto oldType = _type;
        auto oldAccess = _access;
        auto oldAssignment = _assignment;
        auto oldFlags = _flags;
        try
        {
            setName(name);
            setReturnType(returnType);
            setType(type);
            setAccess(access);
            setAssignment(assignment);
            setFlags(flags);
            check();
            updateDisplayIcon();
            updateDisplayText();
        }
        catch (Exception::Block::Logical& e)
        {
            setName(oldName);
            setReturnType(oldReturnType);
            setType(oldType);
            setAccess(oldAccess);
            setAssignment(oldAssignment);
            setFlags(oldFlags);
            throw;
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


void Function::setTemplates(
    const QStringList& value
)
{
    if (_templates != value)
    {
        _templates = value;
        emit templatesChanged(value);
        updateDisplayText();
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
    if (isStatic())
    {
        words.append("static");
    }
    if (isVirtual())
    {
        words.append("virtual");
    }
    if (isExplicit())
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
    if (isNoExcept())
    {
        words.append("noexcept");
    }
    if (isOverride())
    {
        words.append("override");
    }
    if (isFinal())
    {
        words.append("final");
    }
}


void Function::appendRightSignatureFlags(
    QStringList& words
) const
{
    if (isConstant())
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
        if (isDestructor())
        {
            words.append("~"+parentBlock->name()+"("+arguments(true).join(",")+")");
        }
        else
        {
            words.append(parentBlock->name()+"("+arguments(true).join(",")+")");
        }
        break;
    }
    case OperatorFunctionType:
        words.append("operator"+name()+"("+arguments(true).join(",")+")");
        break;
    }
}


void Function::check(
)
{
    static const int allFlags {
        NoExceptFunctionFlag
        |ExplicitFunctionFlag
        |StaticFunctionFlag
        |ConstantFunctionFlag
        |VirtualFunctionFlag
        |OverrideFunctionFlag
        |FinalFunctionFlag
    };
    static const int virtualFlags = VirtualFunctionFlag|OverrideFunctionFlag|FinalFunctionFlag;
    static const QRegularExpression validName("^[a-zA-Z_]+[a-zA-Z_0-9]*$");
    using Error = Exception::Block::Logical;
    if (
        access() < 0
        || access() >= UserAccess
    )
    {
        throw Error(tr("Unkonwn C++ access encountered!"));
    }
    if (
        assignment() < 0
        || assignment() >= UserFunctionAssignment
    )
    {
        throw Error(tr("Unknown function assignment encountered!"));
    }
    if (flags()&(!allFlags))
    {
        throw Error(tr("Unknown function flag encountered!"));
    }
    switch (type())
    {
    case RegularFunctionType:
        if (name().isEmpty())
        {
            throw Error(tr("Functions must have a name."));
        }
        if (!validName.match(name()).hasMatch())
        {
            throw Error(tr("Invalid name '%1' for Function."));
        }
        if (returnType().isEmpty())
        {
            throw Error(tr("Functions must have a return type."));
        }
        if (access() != PublicAccess)
        {
            throw Error(tr("Functions must have public access."));
        }
        if (assignment() != NoFunctionAssignment)
        {
            throw Error(tr("Functions cannot have an assignment."));
        }
        if (flags()&(~ExplicitFunctionFlag))
        {
            throw Error(tr("Functions cannot have specifiers beside 'No Exceptions'."));
        }
        break;
    case MethodFunctionType:
        if (name().isEmpty())
        {
            throw Error(tr("Methods must have a name."));
        }
        if (!validName.match(name()).hasMatch())
        {
            throw Error(tr("Invalid name '%1' for Method."));
        }
        if (returnType().isEmpty())
        {
            throw Error(tr("Methods must have a return type."));
        }
        if (
            assignment() == DefaultFunctionAssignment
            || assignment() == DeleteFunctionAssignment
        )
        {
            throw Error(tr("Methods cannot be assigned default/deleted."));
        }
        if (
            assignment() == AbstractFunctionAssignment
            && !isVirtual()
        )
        {
            throw Error(tr("Abstract method must be virtual."));
        }
        if (
            flags()&(OverrideFunctionFlag|FinalFunctionFlag)
            && !isVirtual()
        )
        {
            throw Error(tr("Methods with override/final specifiers must be virtual."));
        }
        if (
            isFinal()
            && !isOverride()
        )
        {
            throw Error(tr("Methods with final specifier must also have override."));
        }
        if (isExplicit())
        {
            throw Error(tr("Methods cannot be explicit."));
        }
        break;
    case OperatorFunctionType:
        if (name().isEmpty())
        {
            throw Error(tr("Methods must have a name."));
        }
        if (returnType().isEmpty())
        {
            throw Error(tr("Methods must have a return type."));
        }
        if (
            assignment() == DefaultFunctionAssignment
            || assignment() == DeleteFunctionAssignment
        )
        {
            throw Error(tr("Operators cannot be assigned default/deleted."));
        }
        if (
            flags()&virtualFlags
            || assignment() == AbstractFunctionAssignment
        )
        {
            throw Error(tr("Operators cannot be virtual/abstract."));
        }
        if (isExplicit())
        {
            throw Error(tr("Operators cannot be explicit."));
        }
        break;
    case ConstructorFunctionType:
        if (!name().isEmpty())
        {
            throw Error(tr("Constructors cannot have a name."));
        }
        if (!returnType().isEmpty())
        {
            throw Error(tr("Constructors cannot have a return type."));
        }
        if (
            flags()&virtualFlags
            || assignment() == AbstractFunctionAssignment
        )
        {
            throw Error(tr("Constructors cannot be virtual/abstract."));
        }
        break;
    case DestructorFunctionType:
        if (!name().isEmpty())
        {
            throw Error(tr("Destructors cannot have a name."));
        }
        if (!returnType().isEmpty())
        {
            throw Error(tr("Destructors cannot have a return type."));
        }
        if (
            assignment() == AbstractFunctionAssignment
            && !isVirtual()
        )
        {
            throw Error(tr("Abstract destructor must be virtual."));
        }
        if (
            flags()&(OverrideFunctionFlag|FinalFunctionFlag)
            && !isVirtual()
        )
        {
            throw Error(tr("Destructors with override/final specifiers must be virtual."));
        }
        if (
            isFinal()
            && !isOverride()
        )
        {
            throw Error(tr("Destructors with final specifier must also have override."));
        }
        if (isExplicit())
        {
            throw Error(tr("Destructors cannot be explicit."));
        }
        break;
    default:
        throw Error(tr("Unknown function type encountered!"));
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
            _returnType = "";
            return ConstructorFunctionType;
        }
        else if (name() == "~^")
        {
            setName("");
            _returnType = "";
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
        isConstructor()
        || isDestructor()
    )
    {
        updateDisplayText();
    }
}


void Function::onNameChanged(
    const QString& value
)
{
    Q_UNUSED(value);
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
        if (auto p = qobject_cast<Class*>(parent()))
        {
            p->updateDisplayIcon();
        }
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
                return;
            case ProtectedAccess:
                setDisplayIcon(iconAbstractProtected());
                return;
            }
            break;
        case DestructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconAbstractDestructorPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconAbstractDestructorProtected());
                return;
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
                return;
            case ProtectedAccess:
                setDisplayIcon(iconVirtualProtected());
                return;
            }
            break;
        case DestructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconVirtualDestructorPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconVirtualDestructorProtected());
                return;
            }
            break;
        }
    }
    else
    {
        switch (_type)
        {
        case RegularFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconPublic());
                return;
            }
            break;
        case MethodFunctionType:
            if (isStatic())
            {
                switch(_access)
                {
                case PublicAccess:
                    setDisplayIcon(iconStaticPublic());
                    return;
                case ProtectedAccess:
                    setDisplayIcon(iconStaticProtected());
                    return;
                case PrivateAccess:
                    setDisplayIcon(iconStaticPrivate());
                    return;
                }
            }
            else
            {
                switch(_access)
                {
                case PublicAccess:
                    setDisplayIcon(iconPublic());
                    return;
                case ProtectedAccess:
                    setDisplayIcon(iconProtected());
                    return;
                case PrivateAccess:
                    setDisplayIcon(iconPrivate());
                    return;
                }
            }
            break;
        case ConstructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconConstructorPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconConstructorProtected());
                return;
            case PrivateAccess:
                setDisplayIcon(iconConstructorPrivate());
                return;
            }
            break;
        case DestructorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconDestructorPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconDestructorProtected());
                return;
            case PrivateAccess:
                setDisplayIcon(iconDestructorPrivate());
                return;
            }
            break;
        case OperatorFunctionType:
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconOperatorPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconOperatorProtected());
                return;
            case PrivateAccess:
                setDisplayIcon(iconOperatorPrivate());
                return;
            }
            break;
        }
    }
    setDisplayIcon(iconInvalid());
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
    static const QIcon ret(":/cpp/private_destructor.svg");
    return &ret;
}


const QIcon* Function::iconDestructorProtected(
)
{
    static const QIcon ret(":/cpp/protected_destructor.svg");
    return &ret;
}


const QIcon* Function::iconDestructorPublic(
)
{
    static const QIcon ret(":/cpp/public_destructor.svg");
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


void Function::updateDisplayText(
)
{
    QStringList left;
    QStringList right;
    if (!_templates.isEmpty())
    {
        right.append("template<"+_templates.join(",")+">");
    }
    appendLeftFlags(right);
    appendReturn(right);
    appendSignature(left);
    appendRightSignatureFlags(left);
    appendRightFlags(left);
    appendAssignment(left);
    if (!right.isEmpty())
    {
        left.append("->");
        left += right;
    }
    auto displayText = left.join(" ");
    if (_displayText != displayText)
    {
        _displayText = displayText;
        emit displayTextChanged(displayText);
    }
}


void Function::setAccess(
    int value
)
{
    if (_access != value)
    {
        _access = value;
        emit accessChanged(value);
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
    }
}
}
}
