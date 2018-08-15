#ifndef CPPQT_PARENT_H
#define CPPQT_PARENT_H
#include "abstractblock.h"
//



namespace CppQt
{
   /*!
    * This is the parent block. This represents a single inheritance class for another 
    * C++ class. This contains the class name being inherited and its access type of 
    * public, protected, or private. 
    */
   class Parent : public AbstractBlock
   {
      Q_OBJECT
   public:
      class Edit;
      class View;
   public:
      virtual int type() const override final;
      virtual const AbstractBlockFactory& factory() const override final;
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual Sut::QPtr<QWidget> makeView() const override final;
      virtual int fieldSize() const override final;
      virtual AbstractBlock::Field fieldType(int index) const override final;
      virtual QVariant field(int index) const override final;
      virtual Sut::QPtr<::Gui::AbstractEdit> makeEdit() override final;
   public:
      /*!
       * Defines all possible access types a parent class can have. 
       */
      enum class Access
      {
         /*!
          * Defines the public type. 
          */
         Public
         /*!
          * Defines the protected type. 
          */
         ,Protected
         /*!
          * Defines the private type. 
          */
         ,Private
      };
      explicit Parent(bool isDefault = false);
      Parent::Access access() const;
      QString accessString() const;
      QString className() const;
   protected:
      /*!
       * Defines the fields this block contains. 
       */
      enum Field
      {
         /*!
          * Defines the access type field. 
          */
         AccessType
         /*!
          * Defines the class name field. 
          */
         ,ClassName
         /*!
          * Defines the total number of fields this block defines. 
          */
         ,Total
      };
      virtual Sut::QPtr<AbstractBlock> makeBlank() const override final;
      virtual int version() const override final;
      virtual QString fieldTag(int index) const override final;
      virtual int fieldIndexOf(const QString& name) const override final;
      virtual void fieldModified(int index) override final;
      virtual void quietlySetField(int index, const QVariant& value) override final;
   private:
      void setClassName(const QString& value);
      /*!
       * List of this block's field tag names that follow the same order as this block's 
       * enumeration of fields. 
       */
      static const QStringList _fields;
      /*!
       * List of access type names that follow the same order as this block's enumeration 
       * of possible access types. 
       */
      static const QStringList _accessNames;
      /*!
       * The access type for this parent block. 
       */
      Access _access {Access::Public};
      /*!
       * The class name for this parent block. 
       */
      QString _className;
   };
}



#endif
