#ifndef CPPQT_FUNCTION_H
#define CPPQT_FUNCTION_H
#include "cppqt_base.h"



namespace CppQt
{
   class Function : public Base
   {
   public:
      Function() = default;
      Function(const QString& name);
      Function(const QString& returnType, const QString& name);
      virtual QString name() const override final;
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override;
      virtual int type() const override final;
      QString returnType() const;
      void setReturnType(const QString& type);
      QStringList operations() const;
      void setOperations(const QStringList& operations);
   private:
      virtual void readData(const QDomElement& data) override;
      virtual QDomElement writeData(QDomDocument& document) const override;
      static const char* _typeTag;
      static const char* _operationTag;
      QString _returnType {"void"};
      QStringList _operations;
   };
}



#endif
