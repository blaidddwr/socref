#ifndef CPPQT_OPERATOR_H
#define CPPQT_OPERATOR_H
#include "cppqt_function.h"



namespace CppQt
{
   class Operator : public Function
   {
      Q_OBJECT
   public:
      explicit Operator() = default;
      explicit Operator(const QString& returnType);
      virtual QString name() const override final;
      virtual int type() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
      QString operation() const;
      void setOperation(const QString& operation);
   protected:
      virtual void readData(const QDomElement& data) override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
      virtual void copyDataFrom(const AbstractBlock* object) override final;
   private:
      static const char* _operationTag;
      QString _operation {"++"};
   };
}



#endif
