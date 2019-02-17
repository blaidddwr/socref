#ifndef GLSL_VARIABLE_H
#define GLSL_VARIABLE_H
#include "glsl_base.h"



/*!
 * Contains the GLSL project type.
 */
namespace GLSL
{
   /*!
    * This is the variable block. This is used for all variables defined in a
    * shader program along with any local variables and function arguments.
    */
   class Variable : public Base
   {
      Q_OBJECT
   public:
      /*!
       */
      Q_INVOKABLE explicit Variable() = default;
      QString typeString() const;
      QStringList layout() const;
   protected:
      virtual Soc::Ut::QPtr<Basic::BlockView> makeBasicView() const override final;
   };
}

#endif
