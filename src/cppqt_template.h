#ifndef CPPQT_TEMPLATE_H
#define CPPQT_TEMPLATE_H
#include "cppqt_variable.h"



namespace CppQt
{
   class Template : public Variable
   {
      Q_OBJECT
   public:
      Template() = default;
      Template(const QString& name);
      Template(const QString& type, const QString& name);
      virtual std::unique_ptr<AbstractBlock> makeCopy() const override final;
      virtual int type() const override final;
   };
}



#endif
