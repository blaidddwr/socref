#include "glsl_shader.h"
#include <socutil/sut_exceptions.h>



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
   // Initialize a static string list of all possible shader types and an enumeration 
   // of the same that matches the order of the string list. 
   static const QStringList names
   {
      "vertex"
      ,"tesselation control"
      ,"tesselation evaluation"
      ,"geometry"
      ,"fragment"
      ,"compute"
   };
   enum {Vertex,TesselationControl,TesselationEvaluation,Geometry,Fragment,Compute};

   // Create a new string and initialize it to this block's base name. 
   QString ret {baseName()};

   // Figure out what shader type this block is and append the appropriate tag, 
   // throwing an exception if the shader type string is not recognized. 
   switch (names.indexOf(shaderType()))
   {
   case Vertex:
      ret += QStringLiteral(" [V]");
      break;
   case TesselationControl:
      ret += QStringLiteral(" [TC]");
      break;
   case TesselationEvaluation:
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
QString Shader::shaderType() const
{
   return getString("type");
}
