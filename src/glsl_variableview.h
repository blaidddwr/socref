#ifndef GLSL_VARIABLEVIEW_H
#define GLSL_VARIABLEVIEW_H
#include "basic_blockview.h"
#include "glsl.h"



/*!
 * Contains the GLSL project type.
 */
namespace GLSL
{
   /*!
    * This is the view class for the variable block. This provides a custom display
    * showing the layout, if any, of this view's variable.
    */
   class VariableView : public Basic::BlockView
   {
      Q_OBJECT
   public:
      explicit VariableView(const Variable* block);
   private slots:
      QString displayLayout();
   };
}

#endif
