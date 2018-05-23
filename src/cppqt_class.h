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
      virtual int type() const override;
      virtual QString name() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual int fieldSize() const override;
      virtual AbstractBlock::Field fieldType(int index) const override;
      virtual QVariant field(int index) const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
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
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override;
      virtual void fieldModified(int index) override;
      virtual void quietlySetField(int index, const QVariant& value) override;
      virtual bool childNameModified(AbstractBlock* child) override final;
      virtual bool childAdded(AbstractBlock* child) override final;
      virtual bool childRemoved(AbstractBlock* child) override final;
      virtual QStringList fields() const;
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
