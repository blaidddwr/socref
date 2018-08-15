#ifndef CPPQT_TEMPLATE_H
#define CPPQT_TEMPLATE_H
#include "cppqt_variable.h"
//



namespace CppQt
{
   /*!
    * This is the template block. This represents template arguments for classes or 
    * functions. 
    */
   class Template : public Variable
   {
      Q_OBJECT
   public:
      class Edit;
      class View;
   public:
      virtual int type() const override final;
      virtual QIcon icon() const override final;
      virtual Sut::QPtr<QWidget> makeView() const override final;
      virtual Sut::QPtr<::Gui::AbstractEdit> makeEdit() override final;
   public:
      explicit Template(bool isDefault = false);
   protected:
      virtual Sut::QPtr<AbstractBlock> makeBlank() const override final;
   };
}



#endif
