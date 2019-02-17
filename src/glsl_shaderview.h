#ifndef GLSL_SHADERVIEW_H
#define GLSL_SHADERVIEW_H
#include "basic_blockview.h"
#include "glsl.h"



/*!
 * Contains the GLSL project type.
 */
namespace GLSL
{
   /*!
    * This is the view class for the shader block. This provides a custom display
    * showing the number of operations for the shader function.
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
