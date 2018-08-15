#ifndef CPPQT_TYPELIST_H
#define CPPQT_TYPELIST_H
#include "cppqt_base.h"
//



namespace CppQt
{
   /*!
    * This is the type list block. This is used to contain C++ defined types and other 
    * type lists to allow for nesting. This container block type is provided to allow 
    * users to better organize all defined types for a C++/Qt project because it is a 
    * hardly typed language and the list of defined types quickly becomes very long. 
    */
   class TypeList : public Base
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
      virtual Sut::QPtr<QWidget> makeView() const override final;
      virtual Sut::QPtr<::Gui::AbstractEdit> makeEdit() override final;
   public:
      explicit TypeList(bool isDefault = false);
   protected:
      virtual Sut::QPtr<AbstractBlock> makeBlank() const override final;
      virtual bool childNameModified(AbstractBlock* child) override final;
      virtual bool childAdded(AbstractBlock* child) override final;
      virtual bool childRemoved(AbstractBlock* child) override final;
      virtual bool checkName(const QString& value) override final;
   };
}



#endif
