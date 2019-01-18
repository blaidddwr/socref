#ifndef GLSL_VARIABLE_H
#define GLSL_VARIABLE_H
#include "glsl_base.h"
//



namespace GLSL
{
   /*!
    * This is the variable block. This is used for all variables defined in a shader 
    * program along with any local variables and function arguments. 
    */
   class Variable : public Base
   {
      Q_OBJECT
   public:
      class View;
   public:
      /*!
       */
      Q_INVOKABLE explicit Variable() = default;
      QString typeString() const;
      QStringList layout() const;
   protected:
      virtual Sut::QPtr<BasicBlock::View> makeBasicView() const override final;
   };
}



#endif
