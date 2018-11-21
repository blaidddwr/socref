#include "cppqt_blockfactory.h"
#include <socutil/sut_exceptions.h>
#include <projectfactory.h>
#include "cppqt_typelist.h"
#include "cppqt_type.h"



using namespace Sut;
using namespace CppQt;
//






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @return See interface docs. 
 */
int BlockFactory::type() const
{
   return ProjectFactory::CppQtType;
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
QString BlockFactory::elementName(int type) const
{
   // Based off the given block type return its element name, throwing an exception 
   // if the given type is unknown. 
   switch (type)
   {
   case TypeListType: return QStringLiteral("typelist");
   case TypeType: return QStringLiteral("type");
   case NamespaceType: return QStringLiteral("namespace");
   case VariableType: return QStringLiteral("variable");
   case FunctionType: return QStringLiteral("function");
   case TemplateType: return QStringLiteral("template");
   case ClassType: return QStringLiteral("class");
   case AccessType: return QStringLiteral("access");
   case OperatorType: return QStringLiteral("operator");
   case SlotType: return QStringLiteral("slot");
   case SignalType: return QStringLiteral("signal");
   case ConstructorType: return QStringLiteral("constructor");
   case DestructorType: return QStringLiteral("destructor");
   case EnumerationType: return QStringLiteral("enumeration");
   case EnumValueType: return QStringLiteral("enumval");
   case ParentType: return QStringLiteral("parent");
   case UsingType: return QStringLiteral("using");
   case FriendType: return QStringLiteral("friend");
   default:
      {
         Exception::InvalidArgument e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Unknown C++/Qt Block type %1.").arg(type));
         throw e;
      }
   }
}






/*!
 * Constructs a new C++/Qt block factory. 
 */
BlockFactory::BlockFactory():
   BasicBlockFactory(QStringLiteral(":/cppqt/defs.xml"))
{}






/*!
 * Implements _BasicBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<BasicBlock> BlockFactory::makeBasicBlock(int type) const
{
   // Based off the given block type return a new instance of that type or a null 
   // pointer if that type does not have an implementation. 
   switch (type)
   {
   case TypeListType: return QPtr<BasicBlock>(new TypeList);
   case TypeType: return QPtr<BasicBlock>(new Type);
   default: return nullptr;
   }
}
