#include "TestBlockCppQtFunction.h"
#include <QtTest>
#include "BlockCppClass.h"
#include "BlockCppQt.h"
#include "BlockCppQtFunction.h"
#include "FactoryLanguage.h"
#include "LanguageAbstract.h"
#include "Test.h"
namespace Test {
namespace Block {
namespace CppQt {
using ClassBlock = ::Block::Cpp::Class;
using FunctionBlock = ::Block::CppQt::Function;
using namespace ::Block::CppQt;


void Function::initTestCase(
)
{
    QVERIFY(!_block);
    QVERIFY(!_parent);
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cppqt");
    QVERIFY(langIndex >= 0);
    auto language = Factory::Language::instance()->get(langIndex);
    _block = qobject_cast<FunctionBlock*>(language->create(FunctionIndex,this));
    _parent = qobject_cast<ClassBlock*>(language->create(ClassIndex,this));
    QVERIFY(_block);
    QVERIFY(_parent);
    _parent->setName("class123");
    QCOMPARE(_parent->name(),"class123");
    _parent->append(_block);
    QCOMPARE(_block->type(),::Block::Cpp::MethodFunctionType);
}


void Function::displayIconProperty(
)
{
    using namespace ::Block::Cpp;
    static const QIcon testIconAbstractSlotProtected(":/cppqt/abstract_protected_slot.svg");
    static const QIcon testIconAbstractSlotPublic(":/cppqt/abstract_public_slot.svg");
    static const QIcon testIconSignal(":/cppqt/signal.svg");
    static const QIcon testIconSlotPrivate(":/cppqt/private_slot.svg");
    static const QIcon testIconSlotProtected(":/cppqt/protected_slot.svg");
    static const QIcon testIconSlotPublic(":/cppqt/public_slot.svg");
    static const QIcon testIconVirtualSlotProtected(":/cppqt/virtual_protected_slot.svg");
    static const QIcon testIconVirtualSlotPublic(":/cppqt/virtual_public_slot.svg");
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QSignalSpy spy(_block,&FunctionBlock::displayIconChanged);
    auto verify = [&spy,this](const QIcon& icon) -> bool
    {
        if (spy.count() != 1) return false;
        auto arguments = spy.takeLast();
        if (arguments.size() != 1) return false;
        if (!areIconsEqual(arguments.at(0).value<QIcon>(),icon)) return false;
        if (!areIconsEqual(_block->displayIcon(),icon)) return false;
        spy.clear();
        return true;
    };
    _block->set(
        "test"
        ,"void"
        ,SlotFunctionType
        ,ProtectedAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(verify(testIconAbstractSlotProtected));
    _block->set(
        "test"
        ,"void"
        ,SlotFunctionType
        ,PublicAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(verify(testIconAbstractSlotPublic));
    _block->set("test","void",SignalFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconSignal));
    _block->set("test","void",SlotFunctionType,PrivateAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconSlotPrivate));
    _block->set("test","void",SlotFunctionType,ProtectedAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconSlotProtected));
    _block->set("test","void",SlotFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconSlotPublic));
    _block->set(
        "test"
        ,"void"
        ,SlotFunctionType
        ,ProtectedAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(verify(testIconVirtualSlotProtected));
    _block->set(
        "test"
        ,"void"
        ,SlotFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(verify(testIconVirtualSlotPublic));
}


void Function::displayTextProperty(
)
{
    using namespace ::Block::Cpp;
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QSignalSpy spy(_block,&FunctionBlock::displayTextChanged);
    auto verify = [&spy,this](const QString& displayText)
    {
        QCOMPARE(spy.count(),1);
        auto arguments = spy.takeLast();
        QCOMPARE(arguments.size(),1);
        QCOMPARE(arguments.at(0).toString(),displayText);
        QCOMPARE(_block->displayText(),displayText);
        spy.clear();
    };
    _block->set("test","void",SignalFunctionType,PublicAccess,NoFunctionAssignment,0);
    verify("test() -> QSignal void");
    _block->set("test","void",SlotFunctionType,PublicAccess,NoFunctionAssignment,0);
    verify("test() -> QSlot void");
    _block->set(
        "test"
        ,"void"
        ,SlotFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
    );
    verify("test() -> QSlot virtual void");
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,QtInvokableFunctionFlag
    );
    verify("test() -> QInvokable void");
}


void Function::isQtInvokable(
)
{
    using namespace ::Block::Cpp;
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isQtInvokable());
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,QtInvokableFunctionFlag
    );
    QVERIFY(_block->isQtInvokable());
}


void Function::isSignal(
)
{
    using namespace ::Block::Cpp;
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isSignal());
    _block->set("test","void",SignalFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(_block->isSignal());
}


void Function::isSlot(
)
{
    using namespace ::Block::Cpp;
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isSlot());
    _block->set("test","void",SlotFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(_block->isSlot());
}


void Function::cleanupTestCase(
)
{
    QVERIFY(_parent);
    QVERIFY(_block);
    QCOMPARE(_block->parent(),_parent);
    delete _parent;
}
}
}
}
