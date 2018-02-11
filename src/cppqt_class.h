#ifndef CPPQT_CLASS_H
#define CPPQT_CLASS_H
#include "cppqt_namespace.h"
#include "cppqt.h"



namespace CppQt
{
   class Class : public Namespace
   {
      Q_OBJECT
   public:
      explicit Class() = default;
      explicit Class(const QString& name);
      virtual QString name() const override final;
      virtual int type() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
      bool isQtObject() const;
      void setQtObject(bool isQtObject);
      bool isVirtual() const;
      bool isAbstract() const;
      bool hasSignalsOrSlots() const;
      QList<Template*> templates() const;
   signals:
      void bodyChanged();
   protected slots:
      virtual void childNameChanged(AbstractBlock* child) override final;
      virtual void childAdded(AbstractBlock* child) override final;
      virtual void childRemoved(AbstractBlock* child) override final;
   protected:
      virtual void readData(const QDomElement& data) override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void copyDataFrom(const AbstractBlock* object) override final;
   private:
      QList<Access*> accessChildren() const;
      static const char* _qtObjectTag;
      bool _qtObject;
   };
}



#endif
