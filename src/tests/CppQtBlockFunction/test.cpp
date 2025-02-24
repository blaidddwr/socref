#include <QtTest>
#include "CppBlock.h"
#include "CppQt.h"
#include "CppQtBlock.h"
#include "CppBlockClass.h"
#include "CppQtBlock.h"
#include "CppQtBlockFunction.h"
#include "CppQtLanguage.h"
#include "../utility.h"
using namespace CppQt::Block;
using namespace CppQt;
using namespace Cpp;
using Class = Cpp::Block::Class;

class TestCppQtBlockFunction: public QObject
{
    Q_OBJECT
    Class* _parent;
    CppQt::Language* _language;
    Function* _block;
private slots:
    void initTestCase();
    void displayIconProperty();
    void displayTextProperty();
    void isQtInvokable();
    void isSignal();
    void isSlot();
    void cleanupTestCase();
};

void TestCppQtBlockFunction::initTestCase()
{
    Q_INIT_RESOURCE(core);
    Q_INIT_RESOURCE(cpp);
    Q_INIT_RESOURCE(cppqt);
    _language = new CppQt::Language;
    _language->setParent(this);
    _block = qobject_cast<Function*>(_language->createBlock(Cpp::Block::FunctionIndex,this));
    _parent = qobject_cast<Class*>(_language->createBlock(Cpp::Block::ClassIndex,this));
    QVERIFY(_block);
    QVERIFY(_parent);
    _parent->setName("class123");
    QCOMPARE(_parent->name(),"class123");
    _parent->append(_block);
    QCOMPARE(_block->type(),MethodFunctionType);
}

void TestCppQtBlockFunction::displayIconProperty()
{
    static const QIcon testIconAbstractSlotProtected(":/cppqt/abstract_protected_slot.svg");
    static const QIcon testIconAbstractSlotPublic(":/cppqt/abstract_public_slot.svg");
    static const QIcon testIconSignal(":/cppqt/signal.svg");
    static const QIcon testIconSlotPrivate(":/cppqt/private_slot.svg");
    static const QIcon testIconSlotProtected(":/cppqt/protected_slot.svg");
    static const QIcon testIconSlotPublic(":/cppqt/public_slot.svg");
    static const QIcon testIconVirtualSlotProtected(":/cppqt/virtual_protected_slot.svg");
    static const QIcon testIconVirtualSlotPublic(":/cppqt/virtual_public_slot.svg");
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QSignalSpy spy(_block,&Function::displayIconChanged);
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

void TestCppQtBlockFunction::displayTextProperty()
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QSignalSpy spy(_block,&Function::displayTextChanged);
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

void TestCppQtBlockFunction::isQtInvokable()
{
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

void TestCppQtBlockFunction::isSignal()
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isSignal());
    _block->set("test","void",SignalFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(_block->isSignal());
}

void TestCppQtBlockFunction::isSlot()
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isSlot());
    _block->set("test","void",SlotFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(_block->isSlot());
}

void TestCppQtBlockFunction::cleanupTestCase()
{
    delete _parent;
    delete _language;
}

QTEST_MAIN(TestCppQtBlockFunction)
#include "test.moc"
