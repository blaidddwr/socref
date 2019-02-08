#ifndef GLSL_SHADERVIEW_H
#define GLSL_SHADERVIEW_H
#include "basic_blockview.h"
#include "glsl.h"
//



namespace GLSL
{
   /*!
    * This is the view class for the shader block. 
    */
   class ShaderView : public Basic::BlockView
   {
      Q_OBJECT
   public:
      explicit ShaderView(const Shader* block);
   private slots:
      QString displayOperations();
   };
}



#endif
