#ifndef CPPQT_CLASS_H
#define CPPQT_CLASS_H
#include "cppqt_namespace.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    */
   class Class : public Namespace
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
      virtual QList<AbstractBlock*> realChildren() const override final;
   public:
      /*!
       */
      explicit Class() = default;
      explicit Class(const QString& name);
      bool isQtObject() const;
      bool isVirtual() const;
      bool isAbstract() const;
      bool hasSignalsOrSlots() const;
      bool hasTemplates() const;
      bool hasAnyTemplates() const;
      QList<Template*> templates() const;
      QList<Parent*> parents() const;
   signals:
      /*!
       */
      void nameChanged();
   protected:
      /*!
       */
      enum Field
      {
         /*!
          */
         QtObject
         /*!
          */
         ,Total
      };
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void fieldModified(int index) override final;
      virtual void quietlySetField(int index, const QVariant& value) override final;
      virtual bool childNameModified(AbstractBlock* child) override final;
      virtual bool childAdded(AbstractBlock* child) override final;
      virtual bool childRemoved(AbstractBlock* child) override final;
      virtual QStringList fields() const override final;
   private:
      QList<Access*> accessChildren() const;
      void setQtObject(bool state);
      /*!
       */
      static const QStringList _fields;
      /*!
       */
      bool _qtObject {false};
   };
}



#endif
