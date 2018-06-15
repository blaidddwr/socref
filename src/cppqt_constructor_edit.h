#ifndef CPPQT_CONSTRUCTOR_EDIT_H
#define CPPQT_CONSTRUCTOR_EDIT_H
#include "cppqt_constructor.h"
#include "cppqt_function_edit.h"
//



namespace CppQt
{
   /*!
    */
   class Constructor::Edit : public Function::Edit
   {
      Q_OBJECT
   public:
      explicit Edit(Constructor* block);
   protected:
      virtual QLayout* leftLayout();
   };
}



#endif
