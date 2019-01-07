#include "glsl_factory.h"
#include <socutil/sut_exceptions.h>
#include "projectfactory.h"
#include "basicblock.h"



using namespace Sut;
using namespace GLSL;
//






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @return See interface docs. 
 */
int Factory::type() const
{
   return ProjectFactory::GLSLType;
}






/*!
 * Implements _AbstractBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
QString Factory::elementName(int type) const
{
   // Based off the given block type return its element name, throwing an exception 
   // if the given type is unknown. 
   switch (type)
   {
   case NamespaceType: return QStringLiteral("namespace");
   case ShaderType: return QStringLiteral("shader");
   case VariableType: return QStringLiteral("variable");
   case StructType: return QStringLiteral("struct");
   case FunctionType: return QStringLiteral("function");
   default:
      {
         Exception::InvalidArgument e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(QObject::tr("Unknown GLSL Block type %1.").arg(type));
         throw e;
      }
   }
}






/*!
 * Constructs a new GLSL block factory. 
 */
Factory::Factory():
   BasicBlockFactory(":/glsl/defs.xml")
{}






/*!
 * Implements _BasicBlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<BasicBlock> Factory::makeBasicBlock(int type) const
{
   return nullptr;
}
