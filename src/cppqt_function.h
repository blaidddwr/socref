#ifndef CPPQT_FUNCTION_H
#define CPPQT_FUNCTION_H
#include "cppqt_base.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    * This is the function block. This represents any global or method function. This 
    * also acts as any specialized function. The specialized functions include 
    * constructors, destructors, operators, qt signals, and qt slots. A function name 
    * of '^' is considered a constructor and a function name of '~^' is considered a 
    * destructor. The properties supported are default, deleted, qt invokable, 
    * explicit, virtual, constant, constant expression, static, no exceptions, 
    * override, final, and abstract. This block type allows variables and templates 
    * added to it that is interpreted as arguments of the function. 
    */
   class Function : public Base
   {
      Q_OBJECT
   public:
      class View;
   public:
      virtual QString name() const override;
      virtual QIcon icon() const override;
   public:
      /*!
       * Constructs a new function block. 
       */
      Q_INVOKABLE explicit Function() = default;
      bool isConstructor() const;
      bool isDestructor() const;
      bool isOperator() const;
      bool isSignal() const;
      bool isSlot() const;
      bool isDefault() const;
      bool isDeleted() const;
      bool isQtInvokable() const;
      bool isExplicit() const;
      bool isVirtual() const;
      bool isConst() const;
      bool isConstExpr() const;
      bool isStatic() const;
      bool isNoExcept() const;
      bool isOverride() const;
      bool isFinal() const;
      bool isAbstract() const;
      QString baseName() const;
      QString returnType() const;
      QString returnDescription() const;
      bool isVoidReturn() const;
      QStringList operations() const;
      bool isMethod() const;
      bool isPrivateMethod() const;
      bool hasAnyTemplates() const;
      bool hasTemplates() const;
      QList<Template*> templates() const;
      QList<Variable*> arguments() const;
   public slots:
      void classNameChanged();
   protected:
      virtual Soc::Ut::QPtr<BasicBlock::View> makeBasicView() const override final;
      virtual bool childAdded(AbstractBlock* child) override final;
      virtual bool childRemoved(AbstractBlock* child) override final;
   private:
      QString attributes() const;
   };
}



#endif
