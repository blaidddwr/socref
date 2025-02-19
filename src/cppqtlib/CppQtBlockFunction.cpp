#include "CppQtBlockFunction.h"
#include <QtGui>
#include "CppBlockProperty.h"
#include "CppQt.h"
#include "Exception.h"
#include "Global.h"
namespace CppQt {
namespace Block {
using namespace Cpp;
using CppFunction = Cpp::Block::Function;
using Property = Cpp::Block::Property;
bool Function::_iconsInitialized {false};
const QIcon* Function::_abstractProtectedSlotIcon {nullptr};
const QIcon* Function::_abstractPublicSlotIcon {nullptr};
const QIcon* Function::_privateSlotIcon {nullptr};
const QIcon* Function::_protectedSlotIcon {nullptr};
const QIcon* Function::_publicSlotIcon {nullptr};
const QIcon* Function::_signalIcon {nullptr};
const QIcon* Function::_virtualProtectedSlotIcon {nullptr};
const QIcon* Function::_virtualPublicSlotIcon {nullptr};


Function::Function(
    Model::Meta::Block* meta
    ,QObject* parent
):
    CppFunction(meta,parent)
{
    Q_ASSERT(_iconsInitialized);
}


const QMap<int,QString>& Function::flagLabelMap(
) const
{
    static QMap<int,QString>* ret = nullptr;
    if (!ret)
    {
        ret = new QMap<int,QString>(CppFunction::flagLabelMap());
        ret->insert(QtInvokableFunctionFlag,"Qt Invokable");
    }
    return *ret;
}


void Function::initializeIcons(
)
{
    if (!_iconsInitialized)
    {
        CppFunction::initializeIcons();
        _abstractProtectedSlotIcon = new QIcon(":/cppqt/abstract_protected_slot.svg");
        _abstractPublicSlotIcon = new QIcon(":/cppqt/abstract_public_slot.svg");
        _privateSlotIcon = new QIcon(":/cppqt/private_slot.svg");
        _protectedSlotIcon = new QIcon(":/cppqt/protected_slot.svg");
        _publicSlotIcon = new QIcon(":/cppqt/public_slot.svg");
        _signalIcon = new QIcon(":/cppqt/signal.svg");
        _virtualProtectedSlotIcon = new QIcon(":/cppqt/virtual_protected_slot.svg");
        _virtualPublicSlotIcon = new QIcon(":/cppqt/virtual_public_slot.svg");
        _iconsInitialized = true;
    }
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


const QList<QIcon>& Function::typeIcons(
) const
{
    static QList<QIcon>* ret = nullptr;
    if (!ret)
    {
        ret = new QList<QIcon>(CppFunction::typeIcons());
        ret->append(*_signalIcon);
        ret->append(QIcon(":/cppqt/slot.svg"));
    }
    return *ret;
}


const QStringList& Function::typeLabels(
) const
{
    static QStringList* ret = nullptr;
    if (!ret)
    {
        ret = new QStringList(CppFunction::typeLabels());
        ret->append("Signal");
        ret->append("Slot");
    }
    return *ret;
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
    CppFunction::appendLeftFlags(words);
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
        CppFunction::appendReturn(words);
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
        CppFunction::appendSignature(words);
        break;
    }
}


void Function::check(
) const
{
    CppFunction::check();
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


AbstractBlock* Function::create(
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
        ret = new QMap<int,QString>(CppFunction::flagStringMap());
        ret->insert(QtInvokableFunctionFlag,"qtinvokable");
    }
    return *ret;
}


void Function::loadAccess(
    const QVariant& value
    ,int version
)
{
    static const QHash<QString,QString> legacyAccessLookup {
        {"Signals","public"}
        ,{"Public Slots","public"}
        ,{"Protected Slots","protected"}
        ,{"Private Slots","private"}
    };
    static const QHash<QString,int> legacyTypeLookup {
        {"Signals",SignalFunctionType}
        ,{"Public Slots",SlotFunctionType}
        ,{"Protected Slots",SlotFunctionType}
        ,{"Private Slots",SlotFunctionType}
    };
    if (version != Socref_Legacy)
    {
        CppFunction::loadAccess(value,version);
        return;
    }
    auto access = value.toString();
    CppFunction::loadAccess(legacyAccessLookup.value(access,access),version);
    setType(legacyTypeLookup.value(access,type()));
}


int Function::loadFlagsLegacy(
    const QMap<QString,QVariant>& map
)
{
    auto ret = CppFunction::loadFlagsLegacy(map);
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
        ret = new QStringList(CppFunction::typeStrings());
        ret->append("signal");
        ret->append("slot");
    }
    return *ret;
}


void Function::updateDisplayIcon(
)
{
    using namespace Cpp::Block;
    switch (type())
    {
    case SignalFunctionType:
        setDisplayIcon(_signalIcon);
        return;
    case SlotFunctionType:
        if (isAbstract())
        {
            switch (access())
            {
            case PublicAccess:
                setDisplayIcon(_abstractPublicSlotIcon);
                return;
            case ProtectedAccess:
                setDisplayIcon(_abstractProtectedSlotIcon);
                return;
            }
        }
        else if (isVirtual())
        {
            switch (access())
            {
            case PublicAccess:
                setDisplayIcon(_virtualPublicSlotIcon);
                return;
            case ProtectedAccess:
                setDisplayIcon(_virtualProtectedSlotIcon);
                return;
            }
        }
        else
        {
            switch (access())
            {
            case PublicAccess:
                setDisplayIcon(_publicSlotIcon);
                return;
            case ProtectedAccess:
                setDisplayIcon(_protectedSlotIcon);
                return;
            case PrivateAccess:
                setDisplayIcon(_privateSlotIcon);
                return;
            }
        }
        break;
    default:
        CppFunction::updateDisplayIcon();
    }
}


void Function::checkSignal(
) const
{
    using LogicalBlock = Exception::LogicalBlock;
    static const QRegularExpression validName("^[a-zA-Z_]+[a-zA-Z_0-9]*$");
    if (
        parent()
        && !qobject_cast<Property*>(parent())
        )
    {
        throw LogicalBlock(tr("Signals must be the child of a class or property."));
    }
    if (name().isEmpty())
    {
        throw LogicalBlock(tr("Qt signals must have a name."));
    }
    if (!validName.match(name()).hasMatch())
    {
        throw LogicalBlock(tr("Invalid name '%1' for Qt signal."));
    }
    if (returnType() != "void")
    {
        throw LogicalBlock(tr("Qt signals must have a return type of void."));
    }
    if (access() != PublicAccess)
    {
        throw LogicalBlock(tr("Qt signals must have public access."));
    }
    if (assignment() != NoFunctionAssignment)
    {
        throw LogicalBlock(tr("Qt signals cannot have an assignment."));
    }
    if (flags())
    {
        throw LogicalBlock(tr("Qt signals cannot have any specifiers."));
    }
}


void Function::checkSlot(
) const
{
    using LogicalBlock = Exception::LogicalBlock;
    static const QRegularExpression validName("^[a-zA-Z_]+[a-zA-Z_0-9]*$");
    if (
        parent()
        && !qobject_cast<Property*>(parent())
        )
    {
        throw LogicalBlock(tr("Slots must be the child of a class or property."));
    }
    if (name().isEmpty())
    {
        throw LogicalBlock(tr("Qt slots must have a name."));
    }
    if (!validName.match(name()).hasMatch())
    {
        throw LogicalBlock(tr("Invalid name '%1' for Qt slot."));
    }
    if (
        assignment() == DefaultFunctionAssignment
        || assignment() == DeleteFunctionAssignment
        )
    {
        throw LogicalBlock(tr("Qt Slots cannot be assigned default/deleted."));
    }
    if (
        assignment() == AbstractFunctionAssignment
        && !isVirtual()
        )
    {
        throw LogicalBlock(tr("Abstract Qt slot must be virtual."));
    }
    if (
        flags()&(OverrideFunctionFlag|FinalFunctionFlag)
        && !isVirtual()
        )
    {
        throw LogicalBlock(tr("Qt slots with override/final specifiers must be virtual."));
    }
    if (
        isFinal()
        && !isOverride()
        )
    {
        throw LogicalBlock(tr("Qt slots with final specifier must also have override."));
    }
    if (
        isVirtual()
        && isPrivate()
        )
    {
        throw LogicalBlock(tr("Qt slots that are virtual cannot be private."));
    }
    if (isExplicit())
    {
        throw LogicalBlock(tr("Qt slots cannot be explicit."));
    }
}
}
}
