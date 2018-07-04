#ifndef CPPQT_FUNCTION_H
#define CPPQT_FUNCTION_H
#include "cppqt_variable.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
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
      QStringList operations() const;
      bool isVoidReturn() const;
      bool isMethod() const;
      bool isPrivateMethod() const;
      bool hasAnyTemplates() const;
      bool hasTemplates() const;
      QList<Template*> templates() const;
      QList<Variable*> arguments() const;
   protected:
      /*!
       */
      enum Field
      {
         /*!
          */
         Default = Base::Field::Total
         /*!
          */
         ,Explicit
         /*!
          */
         ,Virtual
         /*!
          */
         ,Const
         /*!
          */
         ,ConstExpr
         /*!
          */
         ,Static
         /*!
          */
         ,NoExcept
         /*!
          */
         ,Override
         /*!
          */
         ,Final
         /*!
          */
         ,Abstract
         /*!
          */
         ,ReturnType
         /*!
          */
         ,ReturnDescription
         /*!
          */
         ,Operations
         /*!
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
   private:
      void checkTypeSyntax(const QString& value);
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
      void setReturnType(const QString& value);
      /*!
       */
      static const QStringList _fields;
      /*!
       */
      bool _default {false};
      /*!
       */
      bool _explicit {false};
      /*!
       */
      bool _virtual {false};
      /*!
       */
      bool _const {false};
      /*!
       */
      bool _constExpr {false};
      /*!
       */
      bool _static {false};
      /*!
       */
      bool _noExcept {false};
      /*!
       */
      bool _override {false};
      /*!
       */
      bool _final {false};
      /*!
       */
      bool _abstract {false};
      /*!
       */
      QString _returnType;
      /*!
       */
      QString _returnDescription;
      /*!
       */
      QStringList _operations;
   };
}



#endif
