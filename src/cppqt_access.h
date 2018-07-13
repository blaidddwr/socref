#ifndef CPPQT_ACCESS_H
#define CPPQT_ACCESS_H
#include "abstractblock.h"
//



namespace CppQt
{
   /*!
    * This is the access block. This defines access types for C++ classes such as 
    * public or private. This also defines Qt access types for slots and signals. 
    * Almost all block types used with classes must be attached to an access block 
    * instead of direct attachment because of how C++ works. All supported types are 
    * public, protected, private, signals, public slots, protected slots, and private 
    * slots. 
    */
   class Access : public AbstractBlock
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
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual int fieldSize() const override final;
      virtual AbstractBlock::Field fieldType(int index) const override final;
      virtual QVariant field(int index) const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
   public:
      /*!
       * Defines all possible access types. 
       */
      enum class Type
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
         /*!
          * Defines the signals type. 
          */
         ,Signals
         /*!
          * Defines the public slots type. 
          */
         ,PublicSlots
         /*!
          * Defines the protected slots type. 
          */
         ,ProtectedSlots
         /*!
          * Defines the private slots type. 
          */
         ,PrivateSlots
      };
      static bool isSlot(Type value);
      static bool isNormal(Type value);
      bool hasSignals() const;
      bool hasSlots() const;
      bool hasSignalsOrSlots() const;
      bool hasRegular() const;
      bool hasVirtual() const;
      bool hasAbstract() const;
      Access::Type accessType() const;
      QString accessTypeString() const;
   protected:
      /*!
       * Defines the fields this block contains. 
       */
      enum Field
      {
         /*!
          * Defines the access type field. This field is used to save the access type such 
          * as public or private. 
          */
         AccessType
         /*!
          * Defines the total number of fields this block defines. 
          */
         ,Total
      };
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual int version() const override final;
      virtual QString fieldTag(int index) const override final;
      virtual int fieldIndexOf(const QString& name) const override final;
      virtual void fieldModified(int index) override final;
      virtual void quietlySetField(int index, const QVariant& value) override final;
      virtual bool childNameModified(AbstractBlock* child) override final;
      virtual bool childAdded(AbstractBlock* child) override final;
      virtual bool childRemoved(AbstractBlock* child) override final;
   private:
      void setAccessType(Type value);
      /*!
       * List of this block's field tag names that follow the same order as this block's 
       * enumeration of fields. 
       */
      static const QStringList _fields;
      /*!
       * List of access type names that follow the same order as this block's enumeration 
       * of possible types. 
       */
      static const QStringList _typeNames;
      /*!
       * This block's access type. 
       */
      Type _type {Type::Public};
   };
}



#endif
