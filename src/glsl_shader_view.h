#ifndef GLSL_SHADER_VIEW_H
#define GLSL_SHADER_VIEW_H
#include "glsl_shader.h"
#include "basicblock_view.h"
//



namespace GLSL
{
   /*!
    * This is the view class for the shader block. 
    */
   class Shader::View : public BasicBlock::View
   {
      Q_OBJECT
   public:
      explicit View(const Shader* block);
   private slots:
      QString displayOperations();
   private:
      /*!
       * Pointer to the shader block that this view displays. 
       */
      const Shader* _block;
   };
}



#endif
