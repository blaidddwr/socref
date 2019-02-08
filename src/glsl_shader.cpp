#include "glsl_shader.h"
#include "glsl_shaderview.h"



using namespace GLSL;
//






/*!
 * Implements _Abstract::Block_ interface. 
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
 * Implements _Basic::Block_ interface. 
 *
 * @return See interface docs. 
 */
Soc::Ut::QPtr<Basic::BlockView> Shader::makeBasicView() const
{
   return new ShaderView(this);
}
