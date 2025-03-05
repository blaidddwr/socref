#include "CppBlockFunction.h"
#include <QtGui>
#include "CppBlock.h"
#include "CppBlockClass.h"
#include "CppBlockProperty.h"
#include "CppBlockVariable.h"
#include "CppBlockWidgetFunctionEdit.h"
#include "Exception.h"
#include "Global.h"
#include "ModelMetaBlock.h"
#define ACCESS "access"
#define ASSIGNMENT "assignment"
#define FLAGS "flags"
#define RETURN_DESCRIPTION "returnDescription"
#define RETURN_TYPE "returnType"
#define TEMPLATES "templates"
#define TYPE "type"
namespace Cpp {
namespace Block {
bool Function::_iconsInitialized {false};
const QIcon* Function::_abstractProtectedDestructorIcon {nullptr};
const QIcon* Function::_abstractProtectedFunctionIcon {nullptr};
const QIcon* Function::_abstractPublicDestructorIcon {nullptr};
const QIcon* Function::_abstractPublicFunctionIcon {nullptr};
const QIcon* Function::_invalidFunctionIcon {nullptr};
const QIcon* Function::_privateConstructorIcon {nullptr};
const QIcon* Function::_privateDestructorIcon {nullptr};
const QIcon* Function::_privateFunctionIcon {nullptr};
const QIcon* Function::_privateOperatorIcon {nullptr};
const QIcon* Function::_protectedConstructorIcon {nullptr};
const QIcon* Function::_protectedDestructorIcon {nullptr};
const QIcon* Function::_protectedFunctionIcon {nullptr};
const QIcon* Function::_protectedOperatorIcon {nullptr};
const QIcon* Function::_publicConstructorIcon {nullptr};
const QIcon* Function::_publicDestructorIcon {nullptr};
const QIcon* Function::_publicFunctionIcon {nullptr};
const QIcon* Function::_publicOperatorIcon {nullptr};
const QIcon* Function::_staticPrivateFunctionIcon {nullptr};
const QIcon* Function::_staticProtectedFunctionIcon {nullptr};
const QIcon* Function::_staticPublicFunctionIcon {nullptr};
const QIcon* Function::_virtualProtectedDestructorIcon {nullptr};
const QIcon* Function::_virtualProtectedFunctionIcon {nullptr};
const QIcon* Function::_virtualPublicDestructorIcon {nullptr};
const QIcon* Function::_virtualPublicFunctionIcon {nullptr};


Function::Function(
    Model::Meta::Block* meta
    ,QObject* parent
):
    Namespace("function",meta,parent)
    ,_displayText("function() -> void")
    ,_icon(_publicFunctionIcon)
{
    Q_ASSERT(_iconsInitialized);
}


Function::~Function(
)
{
    if (auto p = qobject_cast<Property*>(parent()))
    {
        auto index = p->indexOf(this);
        Q_ASSERT(index != -1);
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
        *_publicFunctionIcon
        ,*_protectedFunctionIcon
        ,*_privateFunctionIcon
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


AbstractBlockWidget* Function::createWidget(
) const
{
    return new BlockWidget::FunctionEdit(this);
}


QIcon Function::displayIcon(
) const
{
    Q_ASSERT(_icon);
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
        {ExplicitFunctionFlag,"Explicit"}
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


void Function::initializeIcons(
)
{
    if (!_iconsInitialized)
    {
        _abstractProtectedDestructorIcon = new QIcon(":/cpp/abstract_protected_destructor.svg");
        _abstractProtectedFunctionIcon = new QIcon(":/cpp/abstract_protected_function.svg");
        _abstractPublicDestructorIcon = new QIcon(":/cpp/abstract_public_destructor.svg");
        _abstractPublicFunctionIcon = new QIcon(":/cpp/abstract_public_function.svg");
        _invalidFunctionIcon = new QIcon(":/cpp/invalid_function.svg");
        _privateConstructorIcon = new QIcon(":/cpp/private_constructor.svg");
        _privateDestructorIcon = new QIcon(":/cpp/private_destructor.svg");
        _privateFunctionIcon = new QIcon(":/cpp/private_function.svg");
        _privateOperatorIcon = new QIcon(":/cpp/private_operator.svg");
        _protectedConstructorIcon = new QIcon(":/cpp/protected_constructor.svg");
        _protectedDestructorIcon = new QIcon(":/cpp/protected_destructor.svg");
        _protectedFunctionIcon = new QIcon(":/cpp/protected_function.svg");
        _protectedOperatorIcon = new QIcon(":/cpp/protected_operator.svg");
        _publicConstructorIcon = new QIcon(":/cpp/public_constructor.svg");
        _publicDestructorIcon = new QIcon(":/cpp/public_destructor.svg");
        _publicFunctionIcon = new QIcon(":/cpp/public_function.svg");
        _publicOperatorIcon = new QIcon(":/cpp/public_operator.svg");
        _staticPrivateFunctionIcon = new QIcon(":/cpp/static_private_function.svg");
        _staticProtectedFunctionIcon = new QIcon(":/cpp/static_protected_function.svg");
        _staticPublicFunctionIcon = new QIcon(":/cpp/static_public_function.svg");
        _virtualProtectedDestructorIcon = new QIcon(":/cpp/virtual_protected_destructor.svg");
        _virtualProtectedFunctionIcon = new QIcon(":/cpp/virtual_protected_function.svg");
        _virtualPublicDestructorIcon = new QIcon(":/cpp/virtual_public_destructor.svg");
        _virtualPublicFunctionIcon = new QIcon(":/cpp/virtual_public_function.svg");
        _iconsInitialized = true;
    }
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
    for (int i = 0;i < size();i++)
    {
        if (get(i)->meta()->index() == ExceptionIndex)
        {
            return false;
        }
    }
    return true;
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
        if (auto p = qobject_cast<AbstractBlock*>(parent()))
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
    Namespace::loadFromMap(map,version);
    _returnType = map.value(RETURN_TYPE).toString();
    _returnDescription = map.value(RETURN_DESCRIPTION).toString();
    loadType(map.value(TYPE),version);
    loadAccess(map.value(ACCESS),version);
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
        loadFlags(map.value(FLAGS),version);
        loadAssignment(map.value(ASSIGNMENT),version);
        _templates = map.value(TEMPLATES).toString().split(';',Qt::SkipEmptyParts);
    }
    updateDisplayIcon();
    updateDisplayText();
}


QString Function::realName(
) const
{
    switch (_type)
    {
    case RegularFunctionType:
    case MethodFunctionType:
        return name();
    case ConstructorFunctionType:
    case DestructorFunctionType:
    {
        auto parentBlock = qobject_cast<Class*>(parent());
        QString name;
        if (parentBlock)
        {
            name = parentBlock->name();
        }
        else
        {
            name = tr("!DETACHED!");
        }
        return isDestructor() ? "~"+name : name;
    }
    case OperatorFunctionType:
        return "operator"+name();
    default:
        throw std::logic_error("unknown function type");
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
    auto ret = Namespace::saveToMap();
    ret.insert(TYPE,typeString());
    ret.insert(ACCESS,accessString());
    ret.insert(RETURN_TYPE,_returnType);
    if (!_returnDescription.isEmpty())
    {
        ret.insert(RETURN_DESCRIPTION,_returnDescription);
    }
    if (!_templates.isEmpty())
    {
        ret.insert(TEMPLATES,_templates.join(';'));
    }
    auto flags = flagStrings();
    if (!flags.isEmpty())
    {
        ret.insert(FLAGS,flags.join(";"));
    }
    ret.insert(ASSIGNMENT,assignmentString());
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
        Namespace::name() != name
        || _returnType != returnType
        || _type != type
        || _access != access
        || _assignment != assignment
        || _flags != flags
        )
    {
        auto oldName = Namespace::name();
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
        catch (::Exception::LogicalBlock& e)
        {
            setName(oldName);
            setReturnType(oldReturnType);
            setType(oldType);
            setAccess(oldAccess);
            setAssignment(oldAssignment);
            setFlags(oldFlags);
            throw e;
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
    setDescription(state.value(descriptionKey()).toString());
    set(
        state.value(nameKey()).toString()
        ,state.value(RETURN_TYPE).toString()
        ,state.value(TYPE).toInt()
        ,state.value(ACCESS).toInt()
        ,state.value(ASSIGNMENT).toInt()
        ,state.value(FLAGS).toInt()
        );
    setReturnDescription(state.value(RETURN_DESCRIPTION).toString());
    setTemplates(state.value(TEMPLATES).toStringList());
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


QString Function::signature(
) const
{
    QStringList ret;
    appendSignature(ret);
    appendRightSignatureFlags(ret);
    return ret.join("");
}


QHash<QString,QVariant> Function::state(
) const
{
    auto ret = Namespace::state();
    ret.insert(RETURN_TYPE,_returnType);
    ret.insert(TYPE,_type);
    ret.insert(ACCESS,_access);
    ret.insert(ASSIGNMENT,_assignment);
    ret.insert(FLAGS,_flags);
    ret.insert(RETURN_DESCRIPTION,_returnDescription);
    ret.insert(TEMPLATES,_templates);
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
        connect(p,&Namespace::nameChanged,this,&Function::onClassNameChanged);
        p->updateDisplayIcon();
    }
    else
    {
        setType(RegularFunctionType);
        setAccess(PublicAccess);
        setFlags(0);
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
    words.append(realName()+"("+arguments(true).join(",")+")");
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
    using LogicalBlock = ::Exception::LogicalBlock;
    if (!isTypeValid())
    {
        throw LogicalBlock(tr("Unknown function type encountered!"));
    }
    if (!isAccessValid())
    {
        throw LogicalBlock(tr("Unkonwn function access encountered!"));
    }
    if (!isAssignmentValid())
    {
        throw LogicalBlock(tr("Unknown function assignment encountered!"));
    }
    if (!areFlagsValid())
    {
        throw LogicalBlock(tr("Unknown function flag encountered!"));
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


AbstractBlock* Function::create(
    QObject* parent
) const
{
    return new Function(meta(),parent);
}


const QMap<int,QString>& Function::flagStringMap(
) const
{
    static const QMap<int,QString> ret {
        {ExplicitFunctionFlag,"explicit"}
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
        throw ::Exception::ReadBlock(tr("Unknown C++ access %1.").arg(accessString));
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
        disconnect(p,&Namespace::nameChanged,this,&Function::onClassNameChanged);
    }
}


void Function::setDisplayIcon(
    const QIcon* pointer
)
{
    Q_ASSERT(pointer);
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
            setDisplayIcon(_publicFunctionIcon);
            return;
        case ProtectedAccess:
            setDisplayIcon(_protectedFunctionIcon);
            return;
        case PrivateAccess:
            setDisplayIcon(_privateFunctionIcon);
            return;
        }
        break;
    case MethodFunctionType:
        if (isAbstract())
        {
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(_abstractPublicFunctionIcon);
                return;
            case ProtectedAccess:
                setDisplayIcon(_abstractProtectedFunctionIcon);
                return;
            }
        }
        else if (isVirtual())
        {
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(_virtualPublicFunctionIcon);
                return;
            case ProtectedAccess:
                setDisplayIcon(_virtualProtectedFunctionIcon);
                return;
            }
        }
        else if (isStatic())
        {
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(_staticPublicFunctionIcon);
                return;
            case ProtectedAccess:
                setDisplayIcon(_staticProtectedFunctionIcon);
                return;
            case PrivateAccess:
                setDisplayIcon(_staticPrivateFunctionIcon);
                return;
            }
        }
        else
        {
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(_publicFunctionIcon);
                return;
            case ProtectedAccess:
                setDisplayIcon(_protectedFunctionIcon);
                return;
            case PrivateAccess:
                setDisplayIcon(_privateFunctionIcon);
                return;
            }
        }
        break;
    case OperatorFunctionType:
        switch(_access)
        {
        case PublicAccess:
            setDisplayIcon(_publicOperatorIcon);
            return;
        case ProtectedAccess:
            setDisplayIcon(_protectedOperatorIcon);
            return;
        case PrivateAccess:
            setDisplayIcon(_privateOperatorIcon);
            return;
        }
        break;
    case ConstructorFunctionType:
        switch(_access)
        {
        case PublicAccess:
            setDisplayIcon(_publicConstructorIcon);
            return;
        case ProtectedAccess:
            setDisplayIcon(_protectedConstructorIcon);
            return;
        case PrivateAccess:
            setDisplayIcon(_privateConstructorIcon);
            return;
        }
        break;
    case DestructorFunctionType:
        if (isAbstract())
        {
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(_abstractPublicDestructorIcon);
                return;
            case ProtectedAccess:
                setDisplayIcon(_abstractProtectedDestructorIcon);
                return;
            }
        }
        else if (isVirtual())
        {
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(_virtualPublicDestructorIcon);
                return;
            case ProtectedAccess:
                setDisplayIcon(_virtualProtectedDestructorIcon);
                return;
            }
        }
        else
        {
            switch(_access)
            {
            case PublicAccess:
                setDisplayIcon(_publicDestructorIcon);
                return;
            case ProtectedAccess:
                setDisplayIcon(_protectedDestructorIcon);
                return;
            case PrivateAccess:
                setDisplayIcon(_privateDestructorIcon);
                return;
            }
        }
        break;
    }
    setDisplayIcon(_invalidFunctionIcon);
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
    using LogicalBlock = ::Exception::LogicalBlock;
    static const int virtualFlags = VirtualFunctionFlag|OverrideFunctionFlag|FinalFunctionFlag;
    if (
        parent()
        && !qobject_cast<Class*>(parent())
        )
    {
        throw LogicalBlock(tr("Constructors must be the child of a class."));
    }
    if (!name().isEmpty())
    {
        throw LogicalBlock(tr("Constructors cannot have a name."));
    }
    if (!returnType().isEmpty())
    {
        throw LogicalBlock(tr("Constructors cannot have a return type."));
    }
    if (
        flags()&virtualFlags
        || assignment() == AbstractFunctionAssignment
        )
    {
        throw LogicalBlock(tr("Constructors cannot be virtual/abstract."));
    }
}


void Function::checkDestructor(
) const
{
    using LogicalBlock = ::Exception::LogicalBlock;
    if (
        parent()
        && !qobject_cast<Class*>(parent())
        )
    {
        throw LogicalBlock(tr("Destructors must be the child of a class."));
    }
    if (!name().isEmpty())
    {
        throw LogicalBlock(tr("Destructors cannot have a name."));
    }
    if (!returnType().isEmpty())
    {
        throw LogicalBlock(tr("Destructors cannot have a return type."));
    }
    if (
        assignment() == AbstractFunctionAssignment
        && !isVirtual()
        )
    {
        throw LogicalBlock(tr("Abstract destructor must be virtual."));
    }
    if (
        flags()&(OverrideFunctionFlag|FinalFunctionFlag)
        && !isVirtual()
        )
    {
        throw LogicalBlock(tr("Destructors with override/final flags must be virtual."));
    }
    if (
        isFinal()
        && !isOverride()
        )
    {
        throw LogicalBlock(tr("Destructors with final flag must also have override."));
    }
    if (
        isVirtual()
        && isPrivate()
        )
    {
        throw LogicalBlock(tr("Destructors that are virtual cannot be private."));
    }
    if (isExplicit())
    {
        throw LogicalBlock(tr("Destructors cannot be explicit."));
    }
}


void Function::checkMethod(
) const
{
    using LogicalBlock = ::Exception::LogicalBlock;
    static const QRegularExpression validName("^[a-zA-Z_]+[a-zA-Z_0-9]*$");
    if (
        parent()
        && !qobject_cast<Property*>(parent())
        )
    {
        throw LogicalBlock(tr("Methods must be the child of a class or property."));
    }
    if (name().isEmpty())
    {
        throw LogicalBlock(tr("Methods must have a name."));
    }
    if (!validName.match(name()).hasMatch())
    {
        throw LogicalBlock(tr("Invalid name '%1' for method."));
    }
    if (returnType().isEmpty())
    {
        throw LogicalBlock(tr("Methods must have a return type."));
    }
    if (
        assignment() == DefaultFunctionAssignment
        || assignment() == DeleteFunctionAssignment
        )
    {
        throw LogicalBlock(tr("Methods cannot be assigned default/deleted."));
    }
    if (
        assignment() == AbstractFunctionAssignment
        && !isVirtual()
        )
    {
        throw LogicalBlock(tr("Abstract method must be virtual."));
    }
    if (
        flags()&(OverrideFunctionFlag|FinalFunctionFlag)
        && !isVirtual()
        )
    {
        throw LogicalBlock(tr("Methods with override/final flags must be virtual."));
    }
    if (
        isFinal()
        && !isOverride()
        )
    {
        throw LogicalBlock(tr("Methods with final flag must also have override."));
    }
    if (
        isVirtual()
        && isPrivate()
        )
    {
        throw LogicalBlock(tr("Methods that are virtual cannot be private."));
    }
    if (isExplicit())
    {
        throw LogicalBlock(tr("Methods cannot be explicit."));
    }
}


void Function::checkOperator(
) const
{
    using LogicalBlock = ::Exception::LogicalBlock;
    static const int virtualFlags = VirtualFunctionFlag|OverrideFunctionFlag|FinalFunctionFlag;
    if (name().isEmpty())
    {
        throw LogicalBlock(tr("Operators must have a name."));
    }
    if (returnType().isEmpty())
    {
        throw LogicalBlock(tr("Operators must have a return type."));
    }
    if (
        assignment() == DefaultFunctionAssignment
        || assignment() == DeleteFunctionAssignment
        )
    {
        throw LogicalBlock(tr("Operators cannot be assigned default/deleted."));
    }
    if (
        flags()&virtualFlags
        || assignment() == AbstractFunctionAssignment
        )
    {
        throw LogicalBlock(tr("Operators cannot be virtual/abstract."));
    }
    if (isExplicit())
    {
        throw LogicalBlock(tr("Operators cannot be explicit."));
    }
}


void Function::checkRegular(
) const
{
    using LogicalBlock = ::Exception::LogicalBlock;
    static const QRegularExpression validName("^[a-zA-Z_]+[a-zA-Z_0-9]*$");
    if (qobject_cast<Property*>(parent()))
    {
        throw LogicalBlock(tr("Functions cannot be the child of a class or property."));
    }
    if (name().isEmpty())
    {
        throw LogicalBlock(tr("Functions must have a name."));
    }
    if (!validName.match(name()).hasMatch())
    {
        throw LogicalBlock(tr("Invalid name '%1' for Function."));
    }
    if (returnType().isEmpty())
    {
        throw LogicalBlock(tr("Functions must have a return type."));
    }
    if (access() != PublicAccess)
    {
        throw LogicalBlock(tr("Functions must have public access."));
    }
    if (assignment() != NoFunctionAssignment)
    {
        throw LogicalBlock(tr("Functions cannot have an assignment."));
    }
    if (flags())
    {
        throw LogicalBlock(tr("Functions cannot have any flags."));
    }
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
        throw ::Exception::ReadBlock(
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
    const auto flagStrings = value.toString().split(";",Qt::SkipEmptyParts);
    for (const auto& flagString: flagStrings)
    {
        auto flag = reverseFlagLookup().value(flagString,-1);
        if (flag == -1)
        {
            throw ::Exception::ReadBlock(tr("Unkonwn function flag %1.").arg(flagString));
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
            throw ::Exception::ReadBlock(tr("Unknown C++ function type %1.").arg(typeString));
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


void Function::setName(
    const QString& value
)
{
    Namespace::setName(value);
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
