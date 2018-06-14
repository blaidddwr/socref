#ifndef CPPQT_BASE_H
#define CPPQT_BASE_H
#include "abstractblock.h"
//



namespace CppQt
{
   /*!
    * This is the base block class used by many other concrete block types within the 
    * C++/Qt project type. Because this is not a concrete block type it does not 
    * implement all abstract interfaces of the abstract block class. The two common 
    * fields this base class implements are the name and description fields. Certain 
    * abstract block interfaces are handled exclusively by this base class. 
    * 
    * This base class handles the abstract block interfaces that query for field tag 
    * names and finding their index. This is done by adding a new virtual function 
    * that other blocks using this block must implement. This interface returns the 
    * full list of all field tag names for its block, and should be built up from 
    * previous block classes it inherits from. Also because of this the field 
    * enumeration of other classes using this one should begin their enumeration with 
    * the total size of the previous field enumeration it inherits from. 
    * 
    * The name field has a special interface for checking if it is valid which can be 
    * overridden by inherited classes. It does provide a default interface that will 
    * work in most cases. 
    */
   class Base : public AbstractBlock
   {
      Q_OBJECT
   public:
      class View;
   public:
      virtual const AbstractBlockFactory& factory() const override final;
      virtual QString name() const override;
      virtual int fieldSize() const override;
      virtual AbstractBlock::Field fieldType(int index) const override;
      virtual QVariant field(int index) const override;
   public:
      /*!
       * Constructs a new base object. 
       */
      explicit Base() = default;
      explicit Base(const QString& name);
      QString description() const;
   protected:
      /*!
       * Defines the fields this block contains. 
       */
      enum Field
      {
         /*!
          * Defines the name field. This field is for the basic name of many concrete 
          * blocks. 
          */
         Name
         /*!
          * Defines the description field. This is used by many concrete blocks for its 
          * general description. 
          */
         ,Description
         /*!
          * Defines the total number of fields this block defines. 
          */
         ,Total
      };
      virtual int version() const override final;
      virtual QString fieldTag(int index) const override final;
      virtual int fieldIndexOf(const QString& name) const override final;
      virtual void fieldModified(int index) override;
      virtual void quietlySetField(int index, const QVariant& value) override;
      virtual QStringList fields() const;
      virtual bool checkName(const QString& value);
   private:
      void setName(const QString& value);
      /*!
       * List of this block's field tag names that follow the same order as this block's 
       * enumeration of fields. 
       */
      static const QStringList _fields;
      /*!
       * This block's name field value. 
       */
      QString _name;
      /*!
       * This block's description field value. 
       */
      QString _description;
   };
}



#endif
