#ifndef CPPQT_DESTRUCTOR_EDIT_H
#define CPPQT_DESTRUCTOR_EDIT_H
#include "cppqt_destructor.h"
#include "cppqt_function_edit.h"
//



namespace CppQt
{
   /*!
    */
   class Destructor::Edit : public Function::Edit
   {
      Q_OBJECT
   public:
      explicit Edit(Destructor* block);
   protected:
      virtual QLayout* leftLayout();
   };
}



#endif
