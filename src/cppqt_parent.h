#ifndef CPPQT_PARENT_H
#define CPPQT_PARENT_H
#include "abstractblock.h"
//



namespace CppQt
{
   /*!
    */
   class Parent : public AbstractBlock
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
      enum class Access
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
      };
      /*!
       */
      explicit Parent() = default;
      explicit Parent(const QString& className);
      Parent::Access access() const;
      QString accessString() const;
      QString className() const;
      void setAccess(Access value);
      void setAccess(const QString& value);
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
         ,ClassName
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
   private:
      static void checkClassName(const QString& value);
      void setClassName(const QString& value);
      /*!
       */
      static const QStringList _fields;
      /*!
       */
      static const QStringList _accessNames;
      /*!
       */
      Access _access {Access::Public};
      /*!
       */
      QString _className;
   };
}



#endif
