#ifndef CPPQT_VARIABLE_H
#define CPPQT_VARIABLE_H
#include "cppqt_base.h"



namespace CppQt
{
   class Variable : public Base
   {
      Q_OBJECT
   public:
      Variable() = default;
      Variable(const QString& name);
      Variable(const QString& type, const QString& name);
      virtual QString name() const override final;
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override;
      virtual int type() const override;
      QString variableType();
      void setVariableType(const QString& type);
   protected:
      void copyDataFrom(const Variable& object);
   private:
      virtual void readData(const QDomElement& data) override final;
      virtual QDomElement writeData(QDomDocument& document) const override final;
      void checkTypeSyntax(const QString& type);
      static const char* _typeTag;
      QString _type;
   };
}



#endif
