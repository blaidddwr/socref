#ifndef CPPQT_VARIABLE_H
#define CPPQT_VARIABLE_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    * This is the variable block. This represents any global, class member, or 
    * function argument variable. This supports the constant expression, static, 
    * mutable, and thread local properties. This also supports having an initial 
    * value. 
    */
   class Variable : public Base
   {
      Q_OBJECT
   public:
      virtual QString name() const override final;
      virtual QIcon icon() const override;
   public:
      /*!
       * Constructs a new variable block. 
       */
      Q_INVOKABLE explicit Variable() = default;
      bool isConstExpr() const;
      bool isStatic() const;
      bool isMutable() const;
      bool isThreadLocal() const;
      QString variableType() const;
      bool hasInitializer() const;
      QString initializer() const;
      bool isMember() const;
      bool isArgument() const;
   protected:
      virtual Soc::Ut::QPtr<Basic::BlockView> makeBasicView() const override final;
   private:
      QString attributes() const;
   };
}



#endif
