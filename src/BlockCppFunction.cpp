#include "BlockCppFunction.h"
#include <QtGui>
#include "BlockCppProperty.h"
#include "BlockCppVariable.h"
#include "Exceptions.h"
#include "Global.h"
#include "ModelMetaBlock.h"
#include "WidgetBlockCppFunction.h"
namespace Block {
namespace Cpp {


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
    if (auto p = qobject_cast<Property*>(parent()))
    {
        auto index = p->indexOf(this);
        G_ASSERT(index != -1);
        p->take(index);
    }
}


int Function::access(
) const
{
    return _access;
}


const QList<QIcon>& Function::accessIcons(
) const
{
    static const QList<QIcon> ret {
        *iconPublic()
        ,*iconProtected()
        ,*iconPrivate()
    };
    return ret;
}


const QStringList& Function::accessLabels(
) const
{
    static const QStringList ret {
        "Public"
        ,"Protected"
        ,"Private"
    };
    return ret;
}


QString Function::accessString(
) const
{
    return accessStrings().at(_access);
}


int Function::assignment(
) const
{
    return _assignment;
}


const QList<QIcon>& Function::assignmentIcons(
) const
{
    static const QList<QIcon> ret = {
        QIcon(":/cpp/none.svg")
        ,QIcon(":/cpp/default.svg")
        ,QIcon(":/cpp/deleted.svg")
        ,QIcon(":/cpp/abstract.svg")
    };
    return ret;
}


const QStringList& Function::assignmentLabels(
) const
{
    static const QStringList ret = {
        "None"
        ,"Default"
        ,"Deleted"
        ,"Abstract"
    };
    return ret;
}


QString Function::assignmentString(
) const
{
    return assignmentStrings().at(_assignment);
}


Widget::Block::Abstract* Function::createWidget(
) const
{
    return new Widget::Block::Cpp::Function(this);
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


QString Function::fileName(
) const
{
    QStringList ret;
    appendSignature(ret);
    appendRightSignatureFlags(ret);
    return ret.join("").replace(":",";").replace("*","#").replace("<","[").replace(">","]");
}


const QMap<int,QString>& Function::flagLabelMap(
) const
{
    static const QMap<int,QString> ret {
        {NoExceptFunctionFlag,"No Exceptions"}
        ,{ExplicitFunctionFlag,"Explicit"}
        ,{StaticFunctionFlag,"Static"}
        ,{ConstantFunctionFlag,"Constant"}
        ,{VirtualFunctionFlag,"Virtual"}
        ,{OverrideFunctionFlag,"Override"}
        ,{FinalFunctionFlag,"Final"}
    };
    return ret;
}


QStringList Function::flagStrings(
) const
{
    QStringList ret;
    int mask = 1;
    while (mask <= _flags)
    {
        auto word = flagStringMap().value(_flags&mask);
        if (!word.isNull())
        {
            ret.append(word);
        }
        mask = mask<<1;
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


bool Function::isPropertyMethod(
) const
{
    if (_type == MethodFunctionType)
    {
        if (auto p = qobject_cast<Abstract*>(parent()))
        {
            return p->meta()->index() == PropertyIndex;
        }
    }
    return false;
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
    loadType(map.value("type"),version);
    loadAccess(map.value("access"),version);
    if (version == Socref_Legacy)
    {
        _flags = loadFlagsLegacy(map);
        _assignment = loadAssignmentLegacy(map);
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
        loadFlags(map.value("flags"),version);
        loadAssignment(map.value("assignment"),version);
        _templates = map.value("templates").toString().split(';',Qt::SkipEmptyParts);
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
        ret.insert("templates",_templates.join(';'));
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


void Function::setState(
    const QHash<QString,QVariant>& state
)
{
    setDescription(state.value("description").toString());
    set(
        state.value("name").toString()
        ,state.value("returnType").toString()
        ,state.value("type").toInt()
        ,state.value("access").toInt()
        ,state.value("assignment").toInt()
        ,state.value("flags").toInt()
    );
    setReturnDescription(state.value("returnDescription").toString());
    setTemplates(state.value("templates").toStringList());
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


QHash<QString,QVariant> Function::state(
) const
{
    auto ret = Base::state();
    ret.insert("returnType",_returnType);
    ret.insert("type",_type);
    ret.insert("access",_access);
    ret.insert("assignment",_assignment);
    ret.insert("flags",_flags);
    ret.insert("returnDescription",_returnDescription);
    ret.insert("templates",_templates);
    return ret;
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


const QList<QIcon>& Function::typeIcons(
) const
{
    static const QList<QIcon> ret = {
        QIcon(":/cpp/regular.svg")
        ,QIcon(":/cpp/method.svg")
        ,QIcon(":/cpp/constructor.svg")
        ,QIcon(":/cpp/destructor.svg")
        ,QIcon(":/cpp/operator.svg")
    };
    return ret;
}


const QStringList& Function::typeLabels(
) const
{
    static const QStringList ret = {
        "Regular"
        ,"Method"
        ,"Constructor"
        ,"Destructor"
        ,"Operator"
    };
    return ret;
}


QString Function::typeString(
) const
{
    return typeStrings().at(_type);
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


const QStringList& Function::accessStrings(
) const
{
    static const QStringList ret {
        "public"
        ,"protected"
        ,"private"
    };
    return ret;
}


void Function::addEvent(
    int index
)
{
    Q_UNUSED(index);
    if (auto p = qobject_cast<Property*>(parent()))
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


QStringList Function::arguments(
    bool onlyTypes
) const
{
    QStringList ret;
    for (int i = 0;i < size();i++)
    {
        if (auto var = qobject_cast<Variable*>(get(i)))
        {
            auto arg = var->type();
            if (!onlyTypes)
            {
                arg += " "+var->name();
                if (!var->assignment().isEmpty())
                {
                    arg += " = "+var->assignment();
                }
            }
            ret.append(arg);
        }
    }
    return ret;
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
        auto parentBlock = qobject_cast<Property*>(parent());
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
    default:
        G_ASSERT(false);
        break;
    }
}


const QStringList& Function::assignmentStrings(
) const
{
    static const QStringList ret = {
        "none"
        ,"default"
        ,"delete"
        ,"abstract"
    };
    return ret;
}


void Function::check(
) const
{
    using Error = Exception::Block::Logical;
    if (!isTypeValid())
    {
        throw Error(tr("Unknown function type encountered!"));
    }
    if (!isAccessValid())
    {
        throw Error(tr("Unkonwn function access encountered!"));
    }
    if (!isAssignmentValid())
    {
        throw Error(tr("Unknown function assignment encountered!"));
    }
    if (!areFlagsValid())
    {
        throw Error(tr("Unknown function flag encountered!"));
    }
    switch (type())
    {
    case RegularFunctionType:
        checkRegular();
        break;
    case MethodFunctionType:
        checkMethod();
        break;
    case OperatorFunctionType:
        checkOperator();
        break;
    case ConstructorFunctionType:
        checkConstructor();
        break;
    case DestructorFunctionType:
        checkDestructor();
        break;
    }
}


Block::Abstract* Function::create(
    QObject* parent
) const
{
    return new Function(meta(),parent);
}


const QMap<int,QString>& Function::flagStringMap(
) const
{
    static const QMap<int,QString> ret {
        {NoExceptFunctionFlag,"noexcept"}
        ,{ExplicitFunctionFlag,"explicit"}
        ,{StaticFunctionFlag,"static"}
        ,{ConstantFunctionFlag,"const"}
        ,{VirtualFunctionFlag,"virtual"}
        ,{OverrideFunctionFlag,"override"}
        ,{FinalFunctionFlag,"final"}
    };
    return ret;
}


void Function::loadAccess(
    const QVariant& value
    ,int version
)
{
    auto accessString = value.toString();
    if (version == Socref_Legacy)
    {
        accessString = accessString.toLower();
    }
    _access = accessStrings().indexOf(accessString);
    if (_access == -1)
    {
        throw Exception::Block::Read(tr("Unknown C++ access %1.").arg(accessString));
    }
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
    int index
)
{
    Q_UNUSED(index);
    if (auto p = qobject_cast<Property*>(parent()))
    {
        p->updateDisplayIcon();
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
        if (auto p = qobject_cast<Property*>(parent()))
        {
            p->updateDisplayIcon();
        }
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


const QStringList& Function::typeStrings(
) const
{
    static const QStringList ret = {
        "regular"
        ,"method"
        ,"constructor"
        ,"destructor"
        ,"operator"
    };
    return ret;
}


void Function::updateDisplayIcon(
)
{
    switch (_type)
    {
    case RegularFunctionType:
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
        break;
    case MethodFunctionType:
        if (isAbstract())
        {
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconAbstractPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconAbstractProtected());
                return;
            }
        }
        else if (isVirtual())
        {
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconVirtualPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconVirtualProtected());
                return;
            }
        }
        else if (isStatic())
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
        if (isAbstract())
        {
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconAbstractDestructorPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconAbstractDestructorProtected());
                return;
            }
        }
        else if (isVirtual())
        {
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(iconVirtualDestructorPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconVirtualDestructorProtected());
                return;
            }
        }
        else
        {
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
        }
        break;
    }
    setDisplayIcon(iconInvalid());
}


bool Function::areFlagsValid(
) const
{
    static int allFlags = 0;
    if (!allFlags)
    {
        for (auto i = flagStringMap().begin();i != flagStringMap().end();i++)
        {
            allFlags |= i.key();
        }
    }
    return !(flags()&(~allFlags));
}


void Function::checkConstructor(
) const
{
    using Error = Exception::Block::Logical;
    static const int virtualFlags = VirtualFunctionFlag|OverrideFunctionFlag|FinalFunctionFlag;
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
}


void Function::checkDestructor(
) const
{
    using Error = Exception::Block::Logical;
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
    if (
        isVirtual()
        && isPrivate()
    )
    {
        throw Error(tr("Destructors that are virtual cannot be private."));
    }
    if (isExplicit())
    {
        throw Error(tr("Destructors cannot be explicit."));
    }
}


void Function::checkMethod(
) const
{
    using Error = Exception::Block::Logical;
    static const QRegularExpression validName("^[a-zA-Z_]+[a-zA-Z_0-9]*$");
    if (name().isEmpty())
    {
        throw Error(tr("Methods must have a name."));
    }
    if (!validName.match(name()).hasMatch())
    {
        throw Error(tr("Invalid name '%1' for method."));
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
    if (
        isVirtual()
        && isPrivate()
    )
    {
        throw Error(tr("Methods that are virtual cannot be private."));
    }
    if (isExplicit())
    {
        throw Error(tr("Methods cannot be explicit."));
    }
}


void Function::checkOperator(
) const
{
    using Error = Exception::Block::Logical;
    static const int virtualFlags = VirtualFunctionFlag|OverrideFunctionFlag|FinalFunctionFlag;
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
}


void Function::checkRegular(
) const
{
    using Error = Exception::Block::Logical;
    static const QRegularExpression validName("^[a-zA-Z_]+[a-zA-Z_0-9]*$");
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


bool Function::isAccessValid(
) const
{
    return _access >= 0 && _access < accessStrings().size();
}


bool Function::isAssignmentValid(
) const
{
    return _assignment >= 0 && _assignment < assignmentStrings().size();
}


bool Function::isTypeValid(
) const
{
    return _type >= 0 && _type < typeStrings().size();
}


void Function::loadAssignment(
    const QVariant& value
    ,int version
)
{
    Q_UNUSED(version);
    auto assignmentString = value.toString();
    _assignment = assignmentStrings().indexOf(assignmentString);
    if (_assignment == -1)
    {
        throw Exception::Block::Read(
            tr("Unknown C++ function assignment %1").arg(assignmentString)
        );
    }
}


void Function::loadFlags(
    const QVariant& value
    ,int version
)
{
    Q_UNUSED(version);
    _flags = 0;
    for (const auto& flagString: value.toString().split(";",Qt::SkipEmptyParts))
    {
        auto flag = reverseFlagLookup().value(flagString,-1);
        if (flag == -1)
        {
            throw Exception::Block::Read(tr("Unkonwn function flag %1.").arg(flagString));
        }
        _flags |= flag;
    }
}


void Function::loadType(
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
            _type = ConstructorFunctionType;
        }
        else if (name() == "~^")
        {
            setName("");
            _returnType = "";
            _type = DestructorFunctionType;
        }
        else if (name().startsWith("operator"))
        {
            setName(name().mid(8));
            _type = OperatorFunctionType;
        }
        else if (qobject_cast<Property*>(parent()))
        {
            _type = MethodFunctionType;
        }
        else
        {
            _type = RegularFunctionType;
        }
    }
    else
    {
        auto typeString = value.toString();
        _type = typeStrings().indexOf(typeString);
        if (_type == -1)
        {
            throw Exception::Block::Read(tr("Unknown C++ function type %1.").arg(typeString));
        }
    }
}


const QHash<QString,int>& Function::reverseFlagLookup(
) const
{
    QHash<QString,int>* ret = nullptr;
    if (!ret)
    {
        ret = new QHash<QString,int>;
        auto map = flagStringMap();
        for (auto i = map.begin();i != map.end();i++)
        {
            ret->insert(i.value(),i.key());
        }
    }
    return *ret;
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
}
}
