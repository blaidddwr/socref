#ifndef CPPQT_TEMPLATE_EDIT_H
#define CPPQT_TEMPLATE_EDIT_H
#include "cppqt_template.h"
#include "cppqt_variable_edit.h"
//



namespace CppQt
{
   /*!
    * This is the edit class for the template block. 
    */
   class Template::Edit : public Variable::Edit
   {
      Q_OBJECT
   public:
      explicit Edit(Template* block);
   protected:
      virtual QLayout* layout() override final;
   };
}



#endif
