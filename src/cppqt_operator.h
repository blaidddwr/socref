#ifndef CPPQT_OPERATOR_H
#define CPPQT_OPERATOR_H
#include "cppqt_function.h"
//



namespace CppQt
{
   /*!
    */
   class Operator : public Function
   {
      Q_OBJECT
   public:
      class Edit;
   public:
      virtual int type() const override final;
      virtual QString name() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
   public:
      QString operation() const;
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual bool checkName(const QString& value) override final;
   };
}



#endif
