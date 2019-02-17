#ifndef GLSL_FUNCTION_H
#define GLSL_FUNCTION_H
#include "glsl_base.h"
#include "glsl.h"



/*!
 * Contains the GLSL project type.
 */
namespace GLSL
{
   /*!
    * This is the function block. This represents any subroutine functions that can
    * be defined in a GLSL shader program.
    */
   class Function : public Base
   {
      Q_OBJECT
   public:
      virtual QString name() const override final;
   public:
      /*!
       * Constructs a new function block.
       */
      Q_INVOKABLE explicit Function() = default;
      QString returnType() const;
      QString returnDescription() const;
      QStringList operations() const;
      bool isVoidReturn() const;
      QList<Variable*> arguments() const;
   protected:
      virtual Soc::Ut::QPtr<Basic::BlockView> makeBasicView() const override final;
      virtual bool childAdded(Abstract::Block* child) override final;
      virtual bool childRemoved(Abstract::Block* child) override final;
   };
}

#endif
