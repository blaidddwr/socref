#ifndef CPPQT_ENUMVALUE_H
#define CPPQT_ENUMVALUE_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    */
   class EnumValue : public Base
   {
      Q_OBJECT
   public:
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
      explicit EnumValue() = default;
      explicit EnumValue(const QString& name);
      QString value() const;
   protected:
      /*!
       */
      enum Field
      {
         /*!
          */
         Value
         /*!
          */
         ,Total
      };
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual void fieldModified(int index) override;
      virtual void quietlySetField(int index, const QVariant& value) override;
      virtual QStringList fields() const override final;
   private:
      void setValue(const QString& value);
      /*!
       */
      static const QStringList _fields;
      /*!
       */
      QString _value;
   };
}



#endif
