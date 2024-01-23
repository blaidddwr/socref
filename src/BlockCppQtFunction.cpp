#include "BlockCppQtFunction.h"
#include <QtGui>
#include "BlockCppQt.h"
#include "ExceptionBlockLogical.h"
namespace Block {
namespace CppQt {


Widget::Block::Abstract* Function::createWidget(
    QObject* parent
) const
{
    Q_UNUSED(parent);
    return nullptr;//TODO
}


bool Function::isQtInvokable(
) const
{
    return flags()&QtInvokableFunctionFlag;
}


bool Function::isSignal(
) const
{
    return type() == SignalFunctionType;
}


bool Function::isSlot(
) const
{
    return type() == SlotFunctionType;
}


void Function::appendLeftFlags(
    QStringList& words
) const
{
    if (isSignal())
    {
        words.append("QSignal");
    }
    if (isSlot())
    {
        words.append("QSlot");
    }
    if (isQtInvokable())
    {
        words.append("QInvokable");
    }
    Cpp::Function::appendLeftFlags(words);
}


void Function::appendReturn(
    QStringList& words
) const
{
    switch (type())
    {
    case SignalFunctionType:
    case SlotFunctionType:
        words.append(returnType());
        break;
    default:
        Cpp::Function::appendReturn(words);
    }
}


void Function::appendSignature(
    QStringList& words
) const
{
    switch (type())
    {
    case SignalFunctionType:
    case SlotFunctionType:
        words.append(name()+"("+arguments(true).join(",")+")");
        break;
    default:
        Cpp::Function::appendSignature(words);
        break;
    }
}


void Function::check(
) const
{
    Cpp::Function::check();
    switch (type())
    {
    case SignalFunctionType:
        checkSignal();
        break;
    case SlotFunctionType:
        checkSlot();
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
    static QMap<int,QString>* ret = nullptr;
    if (!ret)
    {
        ret = new QMap<int,QString>(Cpp::Function::flagStringMap());
        ret->insert(QtInvokableFunctionFlag,"qtinvokable");
    }
    return *ret;
}


int Function::loadFlagsLegacy(
    const QMap<QString,QVariant>& map
)
{
    auto ret = Cpp::Function::loadFlagsLegacy(map);
    if (
        map.contains("invokable")
        && map.value("invokable").toBool()
    )
    {
        ret |= QtInvokableFunctionFlag;
    }
    return ret;
}


const QStringList& Function::typeStrings(
) const
{
    static QStringList* ret = nullptr;
    if (!ret)
    {
        ret = new QStringList(Cpp::Function::typeStrings());
        ret->append("signal");
        ret->append("slot");
    }
    return *ret;
}


void Function::updateDisplayIcon(
)
{
    using namespace Cpp;
    switch (type())
    {
    case SignalFunctionType:
        setDisplayIcon(iconSignal());
        return;
    case SlotFunctionType:
        if (isAbstract())
        {
            switch (access())
            {
            case PublicAccess:
                setDisplayIcon(iconAbstractSlotPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconAbstractSlotProtected());
                return;
            }
        }
        else if (isVirtual())
        {
            switch (access())
            {
            case PublicAccess:
                setDisplayIcon(iconVirtualSlotPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconVirtualSlotProtected());
                return;
            }
        }
        else
        {
            switch (access())
            {
            case PublicAccess:
                setDisplayIcon(iconSlotPublic());
                return;
            case ProtectedAccess:
                setDisplayIcon(iconSlotProtected());
                return;
            case PrivateAccess:
                setDisplayIcon(iconSlotPrivate());
                return;
            }
        }
        break;
    default:
        Cpp::Function::updateDisplayIcon();
    }
}


void Function::checkSignal(
) const
{
    using Error = Exception::Block::Logical;
    static const QRegularExpression validName("^[a-zA-Z_]+[a-zA-Z_0-9]*$");
    if (name().isEmpty())
    {
        throw Error(tr("Qt signals must have a name."));
    }
    if (!validName.match(name()).hasMatch())
    {
        throw Error(tr("Invalid name '%1' for Qt signal."));
    }
    if (returnType() != "void")
    {
        throw Error(tr("Qt signals must have a return type of void."));
    }
    if (access() != Cpp::PublicAccess)
    {
        throw Error(tr("Qt signals must have public access."));
    }
    if (assignment() != Cpp::NoFunctionAssignment)
    {
        throw Error(tr("Qt signals cannot have an assignment."));
    }
    if (flags())
    {
        throw Error(tr("Qt signals cannot have any specifiers."));
    }
}


void Function::checkSlot(
) const
{
    using Error = Exception::Block::Logical;
    static const QRegularExpression validName("^[a-zA-Z_]+[a-zA-Z_0-9]*$");
    if (name().isEmpty())
    {
        throw Error(tr("Qt slots must have a name."));
    }
    if (!validName.match(name()).hasMatch())
    {
        throw Error(tr("Invalid name '%1' for Qt slot."));
    }
    if (returnType() != "void")
    {
        throw Error(tr("Qt slots must have a return type of void."));
    }
    if (
        assignment() == Cpp::DefaultFunctionAssignment
        || assignment() == Cpp::DeleteFunctionAssignment
    )
    {
        throw Error(tr("Qt Slots cannot be assigned default/deleted."));
    }
    if (
        assignment() == Cpp::AbstractFunctionAssignment
        && !isVirtual()
    )
    {
        throw Error(tr("Abstract Qt slot must be virtual."));
    }
    if (
        flags()&(Cpp::OverrideFunctionFlag|Cpp::FinalFunctionFlag)
        && !isVirtual()
    )
    {
        throw Error(tr("Qt slots with override/final specifiers must be virtual."));
    }
    if (
        isFinal()
        && !isOverride()
    )
    {
        throw Error(tr("Qt slots with final specifier must also have override."));
    }
    if (
        isVirtual()
        && isPrivate()
    )
    {
        throw Error(tr("Qt slots that are virtual cannot be private."));
    }
    if (isExplicit())
    {
        throw Error(tr("Qt slots cannot be explicit."));
    }
}


const QIcon* Function::iconAbstractSlotProtected(
)
{
    static const QIcon ret(":/cppqt/abstract_protected_slot.svg");
    return &ret;
}


const QIcon* Function::iconAbstractSlotPublic(
)
{
    static const QIcon ret(":/cppqt/abstract_public_slot.svg");
    return &ret;
}


const QIcon* Function::iconSignal(
)
{
    static const QIcon ret(":/cppqt/signal.svg");
    return &ret;
}


const QIcon* Function::iconSlotPrivate(
)
{
    static const QIcon ret(":/cppqt/private_slot.svg");
    return &ret;
}


const QIcon* Function::iconSlotProtected(
)
{
    static const QIcon ret(":/cppqt/protected_slot.svg");
    return &ret;
}


const QIcon* Function::iconSlotPublic(
)
{
    static const QIcon ret(":/cppqt/public_slot.svg");
    return &ret;
}


const QIcon* Function::iconVirtualSlotProtected(
)
{
    static const QIcon ret(":/cppqt/virtual_protected_slot.svg");
    return &ret;
}


const QIcon* Function::iconVirtualSlotPublic(
)
{
    static const QIcon ret(":/cppqt/virtual_public_slot.svg");
    return &ret;
}
}
}
