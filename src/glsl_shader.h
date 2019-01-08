#ifndef GLSL_SHADER_H
#define GLSL_SHADER_H
#include "glsl_base.h"
//



namespace GLSL
{
   /*!
    * This is the shader block. This represents a shader program for GLSL. Each shader 
    * program source file is assumed to be in its own file whose file name ends with 
    * the shader name. Anything contained in a shader is added to the source file 
    * along with the necessary main function. 
    */
   class Shader : public Base
   {
      Q_OBJECT
   public:
      virtual QString name() const override final;
   public:
      /*!
       * Constructs a new shader bock. 
       */
      Q_INVOKABLE explicit Shader() = default;
      QString shaderType() const;
   };
}



#endif
