#include "glsl_factory.h"
#include "projectfactory.h"
#include "glsl_namespace.h"
#include "glsl_shader.h"
#include "glsl_variable.h"
#include "glsl_struct.h"
#include "glsl_function.h"



namespace GLSL
{






/*!
 * Implements _AbstractBlockFactory_ interface.
 *
 * @return See interface docs.
 */
int Factory::type() const
{
   return ProjectFactory::GLSL;
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
   case Type::Namespace: return QStringLiteral("namespace");
   case Type::Shader: return QStringLiteral("shader");
   case Type::Variable: return QStringLiteral("variable");
   case Type::Struct: return QStringLiteral("struct");
   case Type::Function: return QStringLiteral("function");
   default:
      Q_ASSERT(false);
   }
}






/*!
 * Constructs a new GLSL block factory.
 */
Factory::Factory()
   :
   Basic::BlockFactory(":/glsl/defs.xml")
{
}






/*!
 * Implements _Basic::BlockFactory_ interface.
 *
 * @param type See interface docs.
 *
 * @return See interface docs.
 */
Soc::Ut::QPtr<Basic::Block> Factory::createBasicBlock(int type) const
{
   // Based off the given block type return a new instance of that type or a null
   // pointer if that type does not have an implementation.
   switch (type)
   {
   case Type::Namespace: return Soc::Ut::QPtr<Basic::Block>(new GLSL::Namespace);
   case Type::Shader: return Soc::Ut::QPtr<Basic::Block>(new GLSL::Shader);
   case Type::Variable: return Soc::Ut::QPtr<Basic::Block>(new GLSL::Variable);
   case Type::Struct: return Soc::Ut::QPtr<Basic::Block>(new GLSL::Struct);
   case Type::Function: return Soc::Ut::QPtr<Basic::Block>(new GLSL::Function);
   default: return nullptr;
   }
}

}
