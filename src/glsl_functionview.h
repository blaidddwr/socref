#ifndef GLSL_FUNCTIONVIEW_H
#define GLSL_FUNCTIONVIEW_H
#include "basic_blockview.h"
#include "glsl.h"



/*!
 * Contains the GLSL project type.
 */
namespace GLSL
{
   /*!
    * This is the view class for the function block. This provides custom displays
    * for this function's arguments, its return value, and it number of operations.
    */
   class FunctionView : public Basic::BlockView
   {
      Q_OBJECT
   public:
      explicit FunctionView(const Function* block);
   private slots:
      QString displayArguments();
      QString displayReturn();
      QString displayOperations();
   };
}

#endif
