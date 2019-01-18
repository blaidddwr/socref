#include "glsl_shader.h"
#include <socutil/sut_exceptions.h>
#include "glsl_shader_view.h"



using namespace Sut;
using namespace GLSL;
//






/*!
 * Implements _AbstractBlock_ interface. 
 *
 * @return See interface docs. 
 */
QString Shader::name() const
{
   // Create a new string and initialize it to this block's base name. 
   QString ret {baseName()};

   // Figure out what shader type this block is and append the appropriate tag, 
   // throwing an exception if the shader type string is not recognized. 
   switch (shaderType())
   {
   case Vertex:
      ret += QStringLiteral(" [V]");
      break;
   case TessellationControl:
      ret += QStringLiteral(" [TC]");
      break;
   case TessellationEvaluation:
      ret += QStringLiteral(" [TE]");
      break;
   case Geometry:
      ret += QStringLiteral(" [G]");
      break;
   case Fragment:
      ret += QStringLiteral(" [F]");
      break;
   case Compute:
      ret += QStringLiteral(" [C]");
      break;
   default:
      {
         Exception::LogicError e;
         SUT_MARK_EXCEPTION(e);
         e.setDetails(tr("Shader block has unknown type '%1'.").arg(shaderType()));
         throw e;
      }
   }

   // Return the name of this block. 
   return ret;
}






/*!
 * Returns this block's shader type. 
 *
 * @return This block's shader type. 
 */
Shader::Type Shader::shaderType() const
{
   // Initialize a static string list of all possible shader types that matches the 
   // order of enumerated types.  
   static const QStringList names
   {
      "vertex"
      ,"tessellation control"
      ,"tessellation evaluation"
      ,"geometry"
      ,"fragment"
      ,"compute"
   };

   // Return this block's shader type based off the index of the static string list. 
   return static_cast<Type>(names.indexOf(shaderTypeString()));
}






/*!
 * Returns this block's shader type as a string. 
 *
 * @return This block's shader type as a string. 
 */
QString Shader::shaderTypeString() const
{
   return getString("type");
}






/*!
 * Returns the full list of all operations of this shader block. 
 *
 * @return Full list of all operations for this shader block. 
 */
QStringList Shader::operations() const
{
   return getStringList("operation");
}






/*!
 * Implements _BasicBlock_ interface. 
 *
 * @return See interface docs. 
 */
Sut::QPtr<BasicBlock::View> Shader::makeBasicView() const
{
   return new View(this);
}
