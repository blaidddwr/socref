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
      virtual QString name() const override;
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override;
      virtual int type() const override;
      virtual QIcon icon() const override;
      virtual QList<int> buildList() const override;
      virtual std::unique_ptr<QWidget> makeView() const override;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override;
      bool isConstExpr() const;
      void setConstExpr(bool isConstExpr);
      bool isStatic() const;
      void setStatic(bool isStatic);
      QString variableType() const;
      void setVariableType(const QString& type);
      bool isClassMember() const;
      bool isFunctionArgument() const;
   protected:
      virtual void readData(const QDomElement& data) override;
      virtual QDomElement writeData(QDomDocument& document) const override;
      void copyDataFrom(const Variable& object);
      QString properties() const;
   private:
      void checkTypeSyntax(const QString& type);
      const char* _typeTag {"type"};
      const char* _constExprTag {"constexpr"};
      const char* _staticTag {"static"};
      QString _type;
      bool _constExpr {false};
      bool _static {false};
   };
}



#endif
