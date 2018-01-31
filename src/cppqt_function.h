#ifndef CPPQT_FUNCTION_H
#define CPPQT_FUNCTION_H
#include "cppqt_base.h"
#include "cppqt.h"



namespace CppQt
{
   class Function : public Base
   {
      Q_OBJECT
   public:
      Function() = default;
      Function(const QString& name);
      Function(const QString& returnType, const QString& name);
      virtual QString name() const override final;
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override;
      virtual int type() const override final;
      QString returnType() const;
      void setReturnType(const QString& type);
      QString returnDescription() const;
      void setReturnDescription(const QString& description);
      QStringList operations() const;
      void setOperations(const QStringList& operations);
      QList<Variable*> arguments() const;
   private slots:
      virtual void childNameChanged(AbstractBlock* child) override final;
      virtual void childAdded(AbstractBlock* child) override final;
      virtual void childRemoved(AbstractBlock* child) override final;
   private:
      virtual void readData(const QDomElement& data) override;
      virtual QDomElement writeData(QDomDocument& document) const override;
      static const char* _typeTag;
      static const char* _descriptionTag;
      static const char* _operationTag;
      QString _returnType {"void"};
      QString _returnDescription;
      QStringList _operations;
   };
}



#endif
