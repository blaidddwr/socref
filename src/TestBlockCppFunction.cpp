#include "TestBlockCppFunction.h"
#include <QtTest>
#include "BlockCppClass.h"
#include "BlockCppFunction.h"
#include "BlockCppVariable.h"
#include "FactoryLanguage.h"
#include "Global.h"
#include "TestBase.t.h"
namespace Test {
namespace Block {
namespace Cpp {
using ClassBlock = ::Block::Cpp::Class;
using FunctionBlock = ::Block::Cpp::Function;
using VariableBlock = ::Block::Cpp::Variable;
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
    QCOMPARE(_block->name(),"function");
    QCOMPARE(_block->access(),PublicAccess);
    QCOMPARE(_block->assignment(),NoFunctionAssignment);
    QCOMPARE(_block->type(),RegularFunctionType);
    QCOMPARE(_block->flags(),0);
    QCOMPARE(_block->returnType(),"void");
    QCOMPARE(_block->returnDescription(),"");
    _parent = create<ClassBlock>(ClassIndex);
    _parent->setName("class123");
    QCOMPARE(_parent->name(),"class123");
    _parent->append(_block);
    QCOMPARE(_block->type(),MethodFunctionType);
}


void Function::accessProperty(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QCOMPARE(_block->access(),PublicAccess);
    QSignalSpy spy(_block,&FunctionBlock::accessChanged);
    _block->set("test","void",MethodFunctionType,ProtectedAccess,NoFunctionAssignment,0);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0).toInt(),ProtectedAccess);
    QCOMPARE(_block->access(),ProtectedAccess);
}


void Function::assignmentProperty(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QCOMPARE(_block->assignment(),NoFunctionAssignment);
    QSignalSpy spy(_block,&FunctionBlock::assignmentChanged);
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
    );
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
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(areIconsEqual(_block->displayIcon(),testIconPublic));
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
    _block->set("test","void",MethodFunctionType,ProtectedAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconProtected));
    _block->set("test","void",MethodFunctionType,PrivateAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconPrivate));
    spy.clear();
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PrivateAccess
        ,NoFunctionAssignment
        ,StaticFunctionFlag
    );
    QVERIFY(verify(testIconStaticPrivate));
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,ProtectedAccess
        ,NoFunctionAssignment
        ,StaticFunctionFlag
    );
    QVERIFY(verify(testIconStaticProtected));
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,StaticFunctionFlag
    );
    QVERIFY(verify(testIconStaticPublic));
    _block->set("","",ConstructorFunctionType,PrivateAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconConstructorPrivate));
    _block->set("","",ConstructorFunctionType,ProtectedAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconConstructorProtected));
    _block->set("","",ConstructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconConstructorPublic));
    _block->set("","",DestructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconDestructorPublic));
    _block->set("","",DestructorFunctionType,ProtectedAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconDestructorProtected));
    _block->set("","",DestructorFunctionType,PrivateAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconDestructorPrivate));
    _block->set("++","void",OperatorFunctionType,PrivateAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconOperatorPrivate));
    _block->set("++","void",OperatorFunctionType,ProtectedAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconOperatorProtected));
    _block->set("++","void",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(verify(testIconOperatorPublic));
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(verify(testIconAbstractPublic));
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,ProtectedAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(verify(testIconAbstractProtected));
    _block->set(
        ""
        ,""
        ,DestructorFunctionType
        ,ProtectedAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(verify(testIconAbstractDestructorProtected));
    _block->set(
        ""
        ,""
        ,DestructorFunctionType
        ,PublicAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(verify(testIconAbstractDestructorPublic));
    _block->set("","",DestructorFunctionType,PublicAccess,NoFunctionAssignment,VirtualFunctionFlag);
    QVERIFY(verify(testIconVirtualDestructorPublic));
    _block->set(
        ""
        ,""
        ,DestructorFunctionType
        ,ProtectedAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(verify(testIconVirtualDestructorProtected));
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,ProtectedAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(verify(testIconVirtualProtected));
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(verify(testIconVirtualPublic));
}


void Function::displayTextProperty(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QCOMPARE(_block->displayText(),"test() -> void");
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
    _block->set("testing123","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    verify("testing123() -> void");
    _block->set(
        "testing123"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag|ConstantFunctionFlag
    );
    verify("testing123() const -> virtual void");
    _block->set(
        "testing123"
        ,"int"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag|ConstantFunctionFlag
    );
    verify("testing123() const -> virtual int");
    _block->set(
        "testing123"
        ,"int"
        ,MethodFunctionType
        ,PublicAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag|ConstantFunctionFlag
    );
    verify("testing123() const = 0 -> virtual int");
    _block->set("","",ConstructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    verify("class123()");
    _block->set("","",DestructorFunctionType,PublicAccess,NoFunctionAssignment,VirtualFunctionFlag);
    verify("~class123() -> virtual");
    _block->set("++","void",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    verify("operator++() -> void");
    _block->set("main","int",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    spy.clear();
    auto arg1 = create<VariableBlock>(VariableIndex);
    auto arg2 = create<VariableBlock>(VariableIndex);
    arg1->setType("int");
    arg2->setType("char**");
    _block->append(arg1);
    verify("main(int) -> int");
    _block->append(arg2);
    verify("main(int,char**) -> int");
    _block->move(1,0);
    verify("main(char**,int) -> int");
    delete arg1;
    verify("main(char**) -> int");
    delete arg2;
    verify("main() -> int");
}


void Function::flagsProperty(
)
{
    static const int testFlags {
        VirtualFunctionFlag|OverrideFunctionFlag|FinalFunctionFlag|ConstantFunctionFlag
    };
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QCOMPARE(_block->flags(),0);
    QSignalSpy spy(_block,&FunctionBlock::flagsChanged);
    _block->set("test","void",MethodFunctionType,PublicAccess,AbstractFunctionAssignment,testFlags);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0).toInt(),testFlags);
    QCOMPARE(_block->flags(),testFlags);
}


void Function::isAbstract(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isAbstract());
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,AbstractFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(_block->isAbstract());
}


void Function::isConstant(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isConstant());
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,ConstantFunctionFlag
    );
    QVERIFY(_block->isConstant());
}


void Function::isConstructor(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isConstructor());
    _block->set("","",ConstructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(_block->isConstructor());
}


void Function::isDefault(
)
{
    _block->set("","",ConstructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isDefault());
    _block->set("","",ConstructorFunctionType,PublicAccess,DefaultFunctionAssignment,0);
    QVERIFY(_block->isDefault());
}


void Function::isDeleted(
)
{
    _block->set("","",ConstructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isDeleted());
    _block->set("","",ConstructorFunctionType,PublicAccess,DeleteFunctionAssignment,0);
    QVERIFY(_block->isDeleted());
}


void Function::isDestructor(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isDestructor());
    _block->set("","",DestructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(_block->isDestructor());
}


void Function::isExplicit(
)
{
    _block->set("","",ConstructorFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isExplicit());
    _block->set(
        ""
        ,""
        ,ConstructorFunctionType
        ,PublicAccess
        ,DeleteFunctionAssignment
        ,ExplicitFunctionFlag
    );
    QVERIFY(_block->isExplicit());
}


void Function::isFinal(
)
{
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag|OverrideFunctionFlag
    );
    QVERIFY(!_block->isFinal());
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag|OverrideFunctionFlag|FinalFunctionFlag
    );
    QVERIFY(_block->isFinal());
}


void Function::isMethod(
)
{
    _parent->take(0);
    _block->set("test","void",RegularFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isMethod());
    _parent->append(_block);
    QVERIFY(_block->isMethod());
}


void Function::isNoExcept(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isNoExcept());
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,NoExceptFunctionFlag
    );
    QVERIFY(_block->isNoExcept());
}


void Function::isOperator(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isOperator());
    _block->set("test","void",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(_block->isOperator());
}


void Function::isOverride(
)
{
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(!_block->isOverride());
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag|OverrideFunctionFlag
    );
    QVERIFY(_block->isOverride());
}


void Function::isPrivate(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isPrivate());
    _block->set("test","void",MethodFunctionType,PrivateAccess,NoFunctionAssignment,0);
    QVERIFY(_block->isPrivate());
}


void Function::isProtected(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isProtected());
    _block->set("test","void",MethodFunctionType,ProtectedAccess,NoFunctionAssignment,0);
    QVERIFY(_block->isProtected());
}


void Function::isPublic(
)
{
    _block->set("test","void",MethodFunctionType,PrivateAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isPublic());
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(_block->isPublic());
}


void Function::isStatic(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isStatic());
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,StaticFunctionFlag
    );
    QVERIFY(_block->isStatic());
}


void Function::isVirtual(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QVERIFY(!_block->isVirtual());
    _block->set(
        "test"
        ,"void"
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag
    );
    QVERIFY(_block->isVirtual());
}


void Function::loadFromMap(
)
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QString testReturnType = "void";
    static const QString testReturnDescription = "description";
    static const QStringList testTemplates = {"Class A","Class B"};
    static const QStringList testFlags {"virtual","override"};
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"returnType",testReturnType}
        ,{"returnDescription",testReturnDescription}
        ,{"templates",testTemplates.join(";")}
        ,{"type","method"}
        ,{"access","public"}
        ,{"assignment","none"}
        ,{"flags",testFlags.join(";")}
    };
    _block->set("","",ConstructorFunctionType,PrivateAccess,DefaultFunctionAssignment,0);
    _block->loadFromMap(testData,Socref_1_0);
    QCOMPARE(_block->name(),testName);
    QCOMPARE(_block->description(),testDescription);
    QCOMPARE(_block->returnType(),testReturnType);
    QCOMPARE(_block->returnDescription(),testReturnDescription);
    QCOMPARE(_block->templates(),testTemplates);
    QCOMPARE(_block->type(),MethodFunctionType);
    QCOMPARE(_block->access(),PublicAccess);
    QCOMPARE(_block->assignment(),NoFunctionAssignment);
    QCOMPARE(_block->flags(),VirtualFunctionFlag|OverrideFunctionFlag);
}


void Function::loadFromMapLegacy(
)
{
    static const QString testName = "^";
    static const QString testDescription = "description";
    static const QString testReturnDescription = "description";
    static const QStringList testTemplates {"class A","class B"};
    static const QString testTemplateString = "template <  class A   ,  class B>";
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"returnType","void"}
        ,{"returnDescription",testReturnDescription}
        ,{"template",testTemplateString}
        ,{"access","Public"}
        ,{"default","1"}
        ,{"explicit",1}
    };
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    _block->loadFromMap(testData,Socref_Legacy);
    QCOMPARE(_block->displayText(),"class123() = default -> template<class A,class B> explicit");
    QCOMPARE(_block->name(),"");
    QCOMPARE(_block->description(),testDescription);
    QCOMPARE(_block->returnType(),"");
    QCOMPARE(_block->returnDescription(),testReturnDescription);
    QCOMPARE(_block->templates(),testTemplates);
    QCOMPARE(_block->type(),ConstructorFunctionType);
    QCOMPARE(_block->access(),PublicAccess);
    QCOMPARE(_block->assignment(),DefaultFunctionAssignment);
    QCOMPARE(_block->flags(),ExplicitFunctionFlag);
}


void Function::returnDescriptionProperty(
)
{
    static const QString testDescription = "testing\n1\n2\n3";
    _block->setReturnDescription("");
    QSignalSpy spy(_block,&FunctionBlock::returnDescriptionChanged);
    _block->setReturnDescription(testDescription);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0).toString(),testDescription);
    QCOMPARE(_block->returnDescription(),testDescription);
}


void Function::returnTypeProperty(
)
{
    static const QString testType = "QList<int>";
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QSignalSpy spy(_block,&FunctionBlock::returnTypeChanged);
    _block->set("test",testType,MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0).toString(),testType);
    QCOMPARE(_block->returnType(),testType);
}


void Function::saveToMap(
)
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QString testReturnType = "void";
    static const QString testReturnDescription = "description";
    static const QStringList testTemplates = {"class A","class B"};
    static const QStringList testFlags {"virtual","override"};
    static const QMap<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"returnType",testReturnType}
        ,{"returnDescription",testReturnDescription}
        ,{"templates",testTemplates.join(";")}
        ,{"type","method"}
        ,{"access","public"}
        ,{"assignment","none"}
        ,{"flags",testFlags.join(";")}
    };
    _block->set(
        testName
        ,testReturnType
        ,MethodFunctionType
        ,PublicAccess
        ,NoFunctionAssignment
        ,VirtualFunctionFlag|Block::Cpp::OverrideFunctionFlag
    );
    _block->setDescription(testDescription);
    _block->setReturnDescription(testReturnDescription);
    _block->setTemplates(testTemplates);
    auto data = _block->saveToMap();
    QCOMPARE(data,testData);
}


void Function::setState(
)
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QString testReturnType = "void";
    static const QString testReturnDescription = "description";
    static const QStringList testTemplates = {"class A","class B"};
    static const int testType = MethodFunctionType;
    static const int testAccess = PublicAccess;
    static const int testAssignment = NoFunctionAssignment;
    static const int testFlags = VirtualFunctionFlag|OverrideFunctionFlag;
    static const QHash<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"returnType",testReturnType}
        ,{"returnDescription",testReturnDescription}
        ,{"templates",testTemplates}
        ,{"type",testType}
        ,{"access",testAccess}
        ,{"assignment",testAssignment}
        ,{"flags",testFlags}
    };
    _block->set("","",ConstructorFunctionType,PrivateAccess,DefaultFunctionAssignment,0);
    _block->setState(testData);
    QCOMPARE(_block->name(),testName);
    QCOMPARE(_block->description(),testDescription);
    QCOMPARE(_block->returnType(),testReturnType);
    QCOMPARE(_block->returnDescription(),testReturnDescription);
    QCOMPARE(_block->type(),MethodFunctionType);
    QCOMPARE(_block->access(),PublicAccess);
    QCOMPARE(_block->assignment(),NoFunctionAssignment);
    QCOMPARE(_block->flags(),VirtualFunctionFlag|OverrideFunctionFlag);
}


void Function::state(
)
{
    static const QString testName = "name";
    static const QString testDescription = "description";
    static const QString testReturnType = "void";
    static const QString testReturnDescription = "description";
    static const QStringList testTemplates = {"Class A","Class B"};
    static const int testType = MethodFunctionType;
    static const int testAccess = PublicAccess;
    static const int testAssignment = NoFunctionAssignment;
    static const int testFlags = VirtualFunctionFlag|OverrideFunctionFlag;
    static const QHash<QString,QVariant> testData {
        {"name",testName}
        ,{"description",testDescription}
        ,{"returnType",testReturnType}
        ,{"returnDescription",testReturnDescription}
        ,{"templates",testTemplates}
        ,{"type",testType}
        ,{"access",testAccess}
        ,{"assignment",testAssignment}
        ,{"flags",testFlags}
    };
    _block->set(testName,testReturnType,testType,testAccess,testAssignment,testFlags);
    _block->setDescription(testDescription);
    _block->setReturnDescription(testReturnDescription);
    _block->setTemplates(testTemplates);
    auto data = _block->state();
    QCOMPARE(data,testData);
}


void Function::scopeName(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    //QCOMPARE(_block->scope(),"test()");
    auto arg1 = create<VariableBlock>(VariableIndex);
    auto arg2 = create<VariableBlock>(VariableIndex);
    arg1->setType("int");
    arg2->setType("char**");
    _block->append(arg1);
    //QCOMPARE(_block->scope(),"test(int)");
    _block->append(arg2);
    //QCOMPARE(_block->scope(),"test(int,char**)");
    _block->move(1,0);
    //QCOMPARE(_block->scope(),"test(char**,int)");
    delete arg1;
    //QCOMPARE(_block->scope(),"test(char**)");
    delete arg2;
    //QCOMPARE(_block->scope(),"test()");
}


void Function::templatesProperty(
)
{
    static const QStringList testTemplates {"class A","class B>"};
    QSignalSpy spy(_block,&FunctionBlock::templatesChanged);
    _block->setTemplates(testTemplates);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0),testTemplates);
    QCOMPARE(_block->templates(),testTemplates);
}


void Function::typeProperty(
)
{
    _block->set("test","void",MethodFunctionType,PublicAccess,NoFunctionAssignment,0);
    QSignalSpy spy(_block,&FunctionBlock::typeChanged);
    _block->set("test","void",OperatorFunctionType,PublicAccess,NoFunctionAssignment,0);
    QCOMPARE(spy.count(),1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.size(),1);
    QCOMPARE(arguments.at(0).toInt(),OperatorFunctionType);
    QCOMPARE(_block->type(),OperatorFunctionType);
}


void Function::cleanupTestCase(
)
{
    delete _block;
}
}
}
}
