#ifndef CPPQT_VARIABLE_H
#define CPPQT_VARIABLE_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    */
   class Variable : public Base
   {
      Q_OBJECT
   public:
      /*!
       */
      enum Field
      {
         /*!
          */
         ConstExpr
         /*!
          */
         ,Static
         /*!
          */
         ,Mutable
         /*!
          */
         ,Type
         /*!
          */
         ,Initializer
         /*!
          */
         ,Total
      };
      virtual int type() const override;
      virtual QString name() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual int fieldSize() const override;
      virtual AbstractBlock::Field fieldType(int index) const override;
      virtual QVariant field(int index) const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
   public:
      /*!
       */
      explicit Variable() = default;
      explicit Variable(const QString& name);
      explicit Variable(const QString& type, const QString& name);
      bool isConstExpr() const;
      bool isStatic() const;
      bool isMutable() const;
      QString variableType() const;
      bool hasInitializer() const;
      QString initializer() const;
      bool isMember() const;
      bool isArgument() const;
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual int version() const override;
      virtual QString fieldTag(int index) const override;
      virtual int fieldIndexOf(const QString& name) const override;
      virtual void fieldModified(int index) override;
      virtual void quietlySetField(int index, const QVariant& value) override;
   private:
      void checkTypeSyntax(const QString& value);
      QString attributes() const;
      void setConstExpr(bool state);
      void setStatic(bool state);
      void setMutable(bool state);
      void setType(const QString& value);
      /*!
       */
      static const QStringList _fields;
      /*!
       */
      bool _constExpr {false};
      /*!
       */
      bool _static {false};
      /*!
       */
      bool _mutable {false};
      /*!
       */
      QString _type;
      /*!
       */
      QString _initializer;
   };
}



#endif
