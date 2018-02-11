#ifndef CPPQT_TEMPLATE_H
#define CPPQT_TEMPLATE_H
#include "cppqt_variable.h"



namespace CppQt
{
   class Template : public Variable
   {
      Q_OBJECT
   public:
      explicit Template() = default;
      explicit Template(const QString& name);
      explicit Template(const QString& type, const QString& name);
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override final;
      virtual int type() const override final;
      virtual QIcon icon() const override final;
      virtual QList<int> buildList() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
   private:
      using Variable::isConstExpr;
      using Variable::setConstExpr;
      using Variable::isStatic;
      using Variable::setStatic;
   };
}



#endif
