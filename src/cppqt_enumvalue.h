#ifndef CPPQT_ENUMVALUE_H
#define CPPQT_ENUMVALUE_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    * This is the enumeration value block. This defines a single value for a C++ 
    * enumeration. This also allows for setting itself to a specific integer value. 
    * Because the integer can be another enumeration value or a macro this value is 
    * allowed to be a string. 
    */
   class EnumValue : public Base
   {
      Q_OBJECT
   public:
      class Edit;
      class View;
   public:
      virtual int type() const override final;
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual int fieldSize() const override final;
      virtual AbstractBlock::Field fieldType(int index) const override final;
      virtual QVariant field(int index) const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
   public:
      bool hasValue() const;
      QString value() const;
   protected:
      /*!
       * Defines the fields this block contains in addition to its base fields. 
       */
      enum Field
      {
         /*!
          * Defines the value field. This field is for when an enumeration value is set to a 
          * specific value. It's value is set to extend the list of base fields this class 
          * inherits. 
          */
         Value = Base::Field::Total
         /*!
          * Defines the total number of fields this block defines. 
          */
         ,Total
      };
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void fieldModified(int index) override final;
      virtual void quietlySetField(int index, const QVariant& value) override final;
      virtual QStringList fields() const override final;
   private:
      /*!
       * List of this block's field tag names that follow the same order as this block's 
       * enumeration of fields. This is in addition to the base fields this block 
       * inherits. 
       */
      static const QStringList _fields;
      /*!
       * The set integer value for this enumeration value block, if any. This is a string 
       * because the integer can be defined as another enumeration value or a macro. 
       */
      QString _value;
   };
}



#endif
