#ifndef CPPQT_USING_H
#define CPPQT_USING_H
#include "abstractblock.h"
//



namespace CppQt
{
   /*!
    * This is the using block. This represents using declarations found in C++ 
    * classes. While using declarations can be made outside of classes this block is 
    * limited to declarations made within class declarations. 
    */
   class Using : public AbstractBlock
   {
      Q_OBJECT
   public:
      class Edit;
      class View;
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
      explicit Using(bool isDefault = false);
      QString fieldName() const;
   protected:
      /*!
       * Defines the fields of this block type. 
       */
      enum Field
      {
         /*!
          * Defines the field field. This is the actual using field after the using 
          * declaration. 
          */
         FieldType
         /*!
          * Defines the total number of fields this block defines. 
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
       * List of this block's field tag names that follow the same order as this block's 
       * enumeration of fields. 
       */
      static const QStringList _fields;
      /*!
       * The field of this using block. 
       */
      QString _field;
   };
}



#endif
