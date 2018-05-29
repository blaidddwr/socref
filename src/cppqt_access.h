#ifndef CPPQT_ACCESS_H
#define CPPQT_ACCESS_H
#include "abstractblock.h"
//



namespace CppQt
{
   /*!
    */
   class Access : public AbstractBlock
   {
      Q_OBJECT
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
       */
      enum class Type
      {
         /*!
          */
         Public
         /*!
          */
         ,Protected
         /*!
          */
         ,Private
         /*!
          */
         ,Signals
         /*!
          */
         ,PublicSlots
         /*!
          */
         ,ProtectedSlots
         /*!
          */
         ,PrivateSlots
      };
      static bool isSlot(Type value);
      static bool isNormal(Type value);
      /*!
       */
      explicit Access() = default;
      explicit Access(Type type);
      bool hasSignals() const;
      bool hasSlots() const;
      bool hasSignalsOrSlots() const;
      bool hasRegular() const;
      bool hasVirtual() const;
      bool hasAbstract() const;
      Access::Type accessType() const;
      QString accessTypeString() const;
      void setAccessType(Type value);
      void setAccessType(const QString& value);
   protected:
      /*!
       */
      enum Field
      {
         /*!
          */
         AccessType
         /*!
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
      void quietlySetAccessType(Type value);
      /*!
       */
      static const QStringList _fields;
      /*!
       */
      static const QStringList _typeNames;
      /*!
       */
      Type _type;
   };
}



#endif
