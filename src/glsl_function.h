#ifndef GLSL_FUNCTION_H
#define GLSL_FUNCTION_H
#include "glsl_base.h"
#include "glsl.h"
//



namespace GLSL
{
   /*!
    * This is the function block. This represents any subroutine functions that can be 
    * defined in a GLSL shader program. 
    */
   class Function : public Base
   {
      Q_OBJECT
   public:
      class View;
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
      virtual Sut::QPtr<BasicBlock::View> makeBasicView() const override final;
      virtual bool childAdded(AbstractBlock* child) override final;
      virtual bool childRemoved(AbstractBlock* child) override final;
   };
}



#endif
