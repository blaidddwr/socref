#ifndef CPPQT_TYPELIST_H
#define CPPQT_TYPELIST_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    */
   class TypeList : public Base
   {
      Q_OBJECT
   public:
      virtual int type() const override final;
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
   public:
      /*!
       */
      explicit TypeList() = default;
      explicit TypeList(const QString& name);
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual bool childNameModified(AbstractBlock* child) override final;
      virtual bool childAdded(AbstractBlock* child) override final;
      virtual bool childRemoved(AbstractBlock* child) override final;
   };
}



#endif
