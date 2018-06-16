#ifndef CPPQT_ENUMERATION_H
#define CPPQT_ENUMERATION_H
#include "cppqt_base.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    * This is the enumeration block. This is a container block which contains all 
    * enumeration values for a C++ enumeration. This provides for unnamed, regular, 
    * and class enumerations. 
    */
   class Enumeration : public Base
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
      /*!
       * Constructs a new enumeration block. 
       */
      explicit Enumeration() = default;
      explicit Enumeration(const QString& name);
      bool isClass() const;
      QList<EnumValue*> values() const;
   protected:
      /*!
       * Defines the fields this block contains in addition to its base fields. 
       */
      enum Field
      {
         /*!
          * Defines the class field. Because this expands the base fields this is set to the 
          * the base fields total. 
          */
         Class = Base::Field::Total
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
       * The block's class field value. 
       */
      bool _class {false};
   };
}



#endif
