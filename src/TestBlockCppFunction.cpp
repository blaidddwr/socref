#include "TestBlockCppFunction.h"
#include <QtTest>
#include "BlockCppClass.h"
#include "BlockCppFunction.h"
#include "FactoryLanguage.h"
namespace Test {
namespace Block {
namespace Cpp {
using FunctionBlock = ::Block::Cpp::Function;
using namespace ::Block::Cpp;


void Function::initTestCase(
)
{
    auto factory = Factory::Language::instance();
    QVERIFY(factory);
    auto langIndex = factory->indexFromName("cpp");
    QVERIFY(langIndex >= 0);
    initLanguage(Factory::Language::instance()->get(langIndex));
    _block = create<FunctionBlock>(FunctionIndex);
    QCOMPARE(_block->access(),PublicAccess);
    QCOMPARE(_block->assignment(),NoFunctionAssignment);
    QCOMPARE(_block->type(),RegularFunctionType);
    QCOMPARE(_block->flags(),0);
    QCOMPARE(_block->returnType(),"void");
    QCOMPARE(_block->returnDescription(),"");
}


void Function::accessProperty(
)
{
    _block->setAccess(PublicAccess);
    QCOMPARE(_block->access(),PublicAccess);
    QSignalSpy spy(_block,&FunctionBlock::accessChanged);
    _block->setAccess(ProtectedAccess);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0).toInt(),ProtectedAccess);
    QCOMPARE(_block->access(),ProtectedAccess);
}


void Function::assignmentProperty(
)
{
    _block->setAssignment(NoFunctionAssignment);
    QCOMPARE(_block->assignment(),NoFunctionAssignment);
    QSignalSpy spy(_block,&FunctionBlock::assignmentChanged);
    _block->setAssignment(AbstractFunctionAssignment);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0).toInt(),AbstractFunctionAssignment);
    QCOMPARE(_block->assignment(),AbstractFunctionAssignment);
}


void Function::displayIconProperty(
)
{
    static const QIcon testIconAbstractDestructorProtected(
        ":/cpp/abstract_protected_destructor.svg"
    );
    static const QIcon testIconAbstractDestructorPublic(":/cpp/abstract_public_destructor.svg");
    static const QIcon testIconAbstractProtected(":/cpp/abstract_protected_function.svg");
    static const QIcon testIconAbstractPublic(":/cpp/abstract_public_function.svg");
    static const QIcon testIconConstructorPrivate(":/cpp/private_constructor.svg");
    static const QIcon testIconConstructorProtected(":/cpp/protected_constructor.svg");
    static const QIcon testIconConstructorPublic(":/cpp/public_constructor.svg");
    static const QIcon testIconDestructorPrivate(":/cpp/private_destructor.svg");
    static const QIcon testIconDestructorProtected(":/cpp/protected_destructor.svg");
    static const QIcon testIconDestructorPublic(":/cpp/public_destructor.svg");
    static const QIcon testIconOperatorPrivate(":/cpp/private_operator.svg");
    static const QIcon testIconOperatorProtected(":/cpp/protected_operator.svg");
    static const QIcon testIconOperatorPublic(":/cpp/public_operator.svg");
    static const QIcon testIconPrivate(":/cpp/private_function.svg");
    static const QIcon testIconProtected(":/cpp/protected_function.svg");
    static const QIcon testIconPublic(":/cpp/public_function.svg");
    static const QIcon testIconStaticPrivate(":/cpp/static_private_function.svg");
    static const QIcon testIconStaticProtected(":/cpp/static_protected_function.svg");
    static const QIcon testIconStaticPublic(":/cpp/static_public_function.svg");
    static const QIcon testIconVirtualDestructorProtected(":/cpp/virtual_protected_destructor.svg");
    static const QIcon testIconVirtualDestructorPublic(":/cpp/virtual_public_destructor.svg");
    static const QIcon testIconVirtualProtected(":/cpp/virtual_protected_function.svg");
    static const QIcon testIconVirtualPublic(":/cpp/virtual_public_function.svg");
    auto parentClass = create<::Block::Cpp::Class>(ClassIndex);
    _block->setType(RegularFunctionType);
    _block->setAssignment(NoFunctionAssignment);
    _block->setFlags(0);
    _block->setAccess(PublicAccess);
    QVERIFY(areIconsEqual(_block->displayIcon(),testIconPublic));
    QSignalSpy spy(_block,&FunctionBlock::displayIconChanged);
    auto verify = [&spy,this](const QIcon& icon)
    {
        QCOMPARE(spy.count(),1);
        auto arguments = spy.takeLast();
        QCOMPARE(arguments.size(),1);
        QVERIFY(areIconsEqual(arguments.at(0).value<QIcon>(),icon));
        QVERIFY(areIconsEqual(_block->displayIcon(),icon));
        spy.clear();
    };
    _block->setAccess(ProtectedAccess);
    verify(testIconProtected);
    _block->setAccess(PrivateAccess);
    verify(testIconPrivate);
    parentClass->append(_block);
    spy.clear();
    _block->setFlags(StaticFunctionFlag);
    verify(testIconStaticPrivate);
    _block->setAccess(ProtectedAccess);
    verify(testIconStaticProtected);
    _block->setAccess(PublicAccess);
    verify(testIconStaticPublic);
    _block->setFlags(0);
    verify(testIconPublic);
    _block->setAccess(ProtectedAccess);
    verify(testIconProtected);
    _block->setAccess(PrivateAccess);
    verify(testIconPrivate);
    _block->setType(ConstructorFunctionType);
    verify(testIconConstructorPrivate);
    _block->setAccess(ProtectedAccess);
    verify(testIconConstructorProtected);
    _block->setAccess(PublicAccess);
    verify(testIconConstructorPublic);
    _block->setType(DestructorFunctionType);
    verify(testIconDestructorPublic);
    _block->setAccess(ProtectedAccess);
    verify(testIconDestructorProtected);
    _block->setAccess(PrivateAccess);
    verify(testIconDestructorPrivate);
    _block->setType(OperatorFunctionType);
    verify(testIconOperatorPrivate);
    _block->setAccess(ProtectedAccess);
    verify(testIconOperatorProtected);
    _block->setAccess(PublicAccess);
    verify(testIconOperatorPublic);
    _block->setType(MethodFunctionType);
    _block->setFlags(VirtualFunctionFlag);
    spy.clear();
    _block->setAssignment(AbstractFunctionAssignment);
    verify(testIconAbstractPublic);
    _block->setAccess(ProtectedAccess);
    verify(testIconAbstractProtected);
    _block->setType(DestructorFunctionType);
    verify(testIconAbstractDestructorProtected);
    _block->setAccess(PublicAccess);
    verify(testIconAbstractDestructorPublic);
    _block->setAssignment(NoFunctionAssignment);
    verify(testIconVirtualDestructorPublic);
    _block->setAccess(ProtectedAccess);
    verify(testIconVirtualDestructorProtected);
    _block->setType(MethodFunctionType);
    verify(testIconVirtualProtected);
    _block->setAccess(PublicAccess);
    verify(testIconVirtualPublic);
    parentClass->take(0);
    delete parentClass;
}


void Function::displayTextProperty(
)
{
}


void Function::flagsProperty(
)
{
}


void Function::isAbstract(
)
{
}


void Function::isConstant(
)
{
}


void Function::isConstructor(
)
{
}


void Function::isDefault(
)
{
}


void Function::isDeleted(
)
{
}


void Function::isDestructor(
)
{
}


void Function::isExplicit(
)
{
}


void Function::isFinal(
)
{
}


void Function::isMethod(
)
{
}


void Function::isNoExcept(
)
{
}


void Function::isOperator(
)
{
}


void Function::isOverride(
)
{
}


void Function::isPrivate(
)
{
}


void Function::isProtected(
)
{
}


void Function::isPublic(
)
{
}


void Function::isStatic(
)
{
}


void Function::isVirtual(
)
{
}


void Function::loadFromMap(
)
{
}


void Function::loadFromMapLegacy(
)
{
}


void Function::returnDescriptionProperty(
)
{
}


void Function::returnTypeProperty(
)
{
}


void Function::saveToMap(
)
{
}


void Function::templatesProperty(
)
{
}


void Function::typeProperty(
)
{
}


void Function::cleanupTestCase(
)
{
    delete _block;
}
}
}
}
