#ifndef CPPQT_USING_H
#define CPPQT_USING_H
#include "abstractblock.h"
//



namespace CppQt
{
   /*!
    */
   class Using : public AbstractBlock
   {
      Q_OBJECT
   public:
      virtual int type() const override;
      virtual const AbstractBlockFactory& factory() const override final;
      virtual QString name() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual int fieldSize() const override final;
      virtual AbstractBlock::Field fieldType(int index) const override final;
      virtual QVariant field(int index) const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
   public:
      /*!
       */
      explicit Using() = default;
      explicit Using(const QString& field);
   protected:
      /*!
       */
      enum Field
      {
         /*!
          */
         FieldType
         /*!
          */
         ,Total
      };
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual int version() const override final;
      virtual QString fieldTag(int index) const override final;
      virtual int fieldIndexOf(const QString& name) const override final;
      virtual void fieldModified(int index) override final;
      virtual void quietlySetField(int index, const QVariant& value) override final;
      virtual void checkField(const QString& value);
   protected:
      void setField(const QString& value);
   private:
      /*!
       */
      static const QStringList _fields;
      /*!
       */
      QString _field;
   };
}



#endif
