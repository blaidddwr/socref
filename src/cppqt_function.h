#ifndef CPPQT_FUNCTION_H
#define CPPQT_FUNCTION_H
#include "cppqt_variable.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    * This is the function block. This represents any global or method function. This 
    * also acts as a base class for other specialized C++ and Qt functions such as 
    * constructors or slots to name a few. This base function class contains all 
    * attributes of any specialized type and does not allow its view class to be 
    * overridden because of that. 
    * 
    * The properties supported are default, explicit, virtual, constant, constant 
    * expression, static, no exceptions, override, final, and abstract. This block 
    * will also check basic sanity of properties such as not allowing a function to be 
    * static and virtual at once, throwing an exception if setting a property violates 
    * any checks. Other supported fields are the return type and description. 
    * 
    * This block type allows variables and templates added to it. Templates are sanity 
    * checked with properties, not allowing templates to be added if the function is 
    * virtual. 
    */
   class Function : public Base
   {
      Q_OBJECT
   public:
      class Edit;
      class View;
   public:
      virtual int type() const override;
      virtual QString name() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual int fieldSize() const override final;
      virtual AbstractBlock::Field fieldType(int index) const override final;
      virtual QVariant field(int index) const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
   public:
      explicit Function(bool isDefault = false);
      bool isDefault() const;
      bool isExplicit() const;
      bool isVirtual() const;
      bool isConst() const;
      bool isConstExpr() const;
      bool isStatic() const;
      bool isNoExcept() const;
      bool isOverride() const;
      bool isFinal() const;
      bool isAbstract() const;
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
   protected:
      /*!
       * Defines the fields this block contains in addition to its base fields. 
       */
      enum Field
      {
         /*!
          * Defines the default field. This is a boolean property. Its value is set to 
          * extend the list of base fields this class inherits. 
          */
         Default = Base::Field::Total
         /*!
          * Defines the explicit field. This is a boolean property. 
          */
         ,Explicit
         /*!
          * Defines the virtual field. This is a boolean property. 
          */
         ,Virtual
         /*!
          * Defines the constant field. This is a boolean property. 
          */
         ,Const
         /*!
          * Defines the constant expression field. This is a boolean property. 
          */
         ,ConstExpr
         /*!
          * Defines the static field. This is a boolean property. 
          */
         ,Static
         /*!
          * Defines the no exceptions field. This is a boolean property. 
          */
         ,NoExcept
         /*!
          * Defines the override field. This is a boolean property. 
          */
         ,Override
         /*!
          * Defines the final field. This is a boolean property. 
          */
         ,Final
         /*!
          * Defines the abstract field. This is a boolean property. 
          */
         ,Abstract
         /*!
          * Defines the return type field. This holds the C++ return type for a function 
          * block. 
          */
         ,ReturnType
         /*!
          * Defines the return description field. This holds the return description for a 
          * function block. 
          */
         ,ReturnDescription
         /*!
          * Defines the operations field. This holds the steps of operation of a function 
          * block used for the inline comments. 
          */
         ,Operations
         /*!
          * Defines the total number of fields this block defines. 
          */
         ,Total
      };
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual void fieldModified(int index) override final;
      virtual void quietlySetField(int index, const QVariant& value) override final;
      virtual bool childAdded(AbstractBlock* child) override final;
      virtual bool childRemoved(AbstractBlock* child) override final;
      virtual QStringList fields() const override final;
   protected:
      QString fullName(bool hasReturn, const QString& name) const;
      void setReturnType(const QString& value);
   private:
      QString attributes() const;
      void setDefault(bool state);
      void setExplicit(bool state);
      void setVirtual(bool state);
      void setConst(bool state);
      void setConstExpr(bool state);
      void setStatic(bool state);
      void setOverride(bool state);
      void setFinal(bool state);
      void setAbstract(bool state);
      /*!
       * List of this block's field tag names that follow the same order as this block's 
       * enumeration of fields. This is in addition to the base fields this block 
       * inherits. 
       */
      static const QStringList _fields;
      /*!
       * The default property for this function block. 
       */
      bool _default {false};
      /*!
       * The explicit property for this function block. 
       */
      bool _explicit {false};
      /*!
       * The virtual property for this function block. 
       */
      bool _virtual {false};
      /*!
       * The constant property for this function block. 
       */
      bool _const {false};
      /*!
       * The constant expression property for this function block. 
       */
      bool _constExpr {false};
      /*!
       * The static property for this function block. 
       */
      bool _static {false};
      /*!
       * The no exception property for this function block. 
       */
      bool _noExcept {false};
      /*!
       * The override property for this function block. 
       */
      bool _override {false};
      /*!
       * The final property for this function block. 
       */
      bool _final {false};
      /*!
       * The abstract property for this function block. 
       */
      bool _abstract {false};
      /*!
       * The return type for this function block. 
       */
      QString _returnType {"void"};
      /*!
       * The description for what this function returns. This should be blank if this 
       * function returns nothing. 
       */
      QString _returnDescription;
      /*!
       * The steps of operation used to populate the inline comments of this function 
       * block's code when parsed. 
       */
      QStringList _operations;
   };
}



#endif
