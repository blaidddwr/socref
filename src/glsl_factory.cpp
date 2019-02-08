#include "glsl_factory.h"
#include "projectfactory.h"
#include "glsl_namespace.h"
#include "glsl_shader.h"
#include "glsl_variable.h"
#include "glsl_struct.h"
#include "glsl_function.h"



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
      Q_ASSERT(false);
   }
}






/*!
 * Constructs a new GLSL block factory. 
 */
Factory::Factory():
   Basic::BlockFactory(":/glsl/defs.xml")
{}






/*!
 * Implements _Basic::BlockFactory_ interface. 
 *
 * @param type See interface docs. 
 *
 * @return See interface docs. 
 */
Soc::Ut::QPtr<Basic::Block> Factory::makeBasicBlock(int type) const
{
   // Based off the given block type return a new instance of that type or a null 
   // pointer if that type does not have an implementation. 
   switch (type)
   {
   case NamespaceType: return Soc::Ut::QPtr<Basic::Block>(new Namespace);
   case ShaderType: return Soc::Ut::QPtr<Basic::Block>(new Shader);
   case VariableType: return Soc::Ut::QPtr<Basic::Block>(new Variable);
   case StructType: return Soc::Ut::QPtr<Basic::Block>(new Struct);
   case FunctionType: return Soc::Ut::QPtr<Basic::Block>(new Function);
   default: return nullptr;
   }
}
