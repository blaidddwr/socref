#ifndef CPPQT_TEMPLATE_H
#define CPPQT_TEMPLATE_H
#include "cppqt_variable.h"
//



namespace CppQt
{
   /*!
    */
   class Template : public Variable
   {
      Q_OBJECT
   public:
      virtual int type() const override final;
      virtual QIcon icon() const override final;
      virtual std::unique_ptr<QWidget> makeView() const override final;
      virtual std::unique_ptr<::Gui::AbstractEdit> makeEdit() override final;
   public:
      explicit Template();
      explicit Template(const QString& name);
      explicit Template(const QString& type, const QString& name);
   protected:
      virtual std::unique_ptr<AbstractBlock> makeBlank() const override final;
   };
}



#endif
