#ifndef GLSL_SHADER_H
#define GLSL_SHADER_H
#include "glsl_base.h"



/*!
 * Contains the GLSL project type.
 */
namespace GLSL
{
   /*!
    * This is the shader block. This represents a shader program for GLSL. Each
    * shader program source file is assumed to be in its own file whose file name
    * ends with the shader name. Anything contained in a shader is added to the
    * source file along with the necessary main function.
    */
   class Shader : public Base
   {
      Q_OBJECT
   public:
      /*!
       * This defines all possible shader types.
       */
      enum Type
      {
         /*!
          * This defines the vertex shader type.
          */
         Vertex
         /*!
          * This defines the tessellation control shader type.
          */
         ,TessellationControl
         /*!
          * This defines the tessellation evaluation shader type.
          */
         ,TessellationEvaluation
         /*!
          * This defines the geometry shader type.
          */
         ,Geometry
         /*!
          * This defines the fragment shader type.
          */
         ,Fragment
         /*!
          * This defines the compute shader type.
          */
         ,Compute
      };
   public:
      virtual QString name() const override final;
   public:
      /*!
       * Constructs a new shader bock.
       */
      Q_INVOKABLE explicit Shader() = default;
      Shader::Type shaderType() const;
      QString shaderTypeString() const;
      QStringList operations() const;
   protected:
      virtual Soc::Ut::QPtr<Basic::BlockView> makeBasicView() const override final;
   };
}

#endif
