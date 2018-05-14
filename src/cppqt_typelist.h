#ifndef CPPQT_TYPELIST_H
#define CPPQT_TYPELIST_H
#include "cppqt_base.h"



namespace CppQt
{
   class TypeList : public Base
   {
      Q_OBJECT
   public:
      explicit TypeList() = default;
      explicit TypeList(const QString& name);
      virtual int type() const override;
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
   protected slots:
      virtual void childNameChanged(AbstractBlock* child) override final;
      virtual void childAdded(AbstractBlock* child) override final;
      virtual void childRemoved(AbstractBlock* child) override final;
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
   };
}



#endif
