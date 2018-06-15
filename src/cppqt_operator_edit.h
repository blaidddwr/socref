#ifndef CPPQT_OPERATOR_EDIT_H
#define CPPQT_OPERATOR_EDIT_H
#include "cppqt_operator.h"
#include "cppqt_function_edit.h"
//



namespace CppQt
{
   /*!
    */
   class Operator::Edit : public Function::Edit
   {
      Q_OBJECT
   public:
      explicit Edit(Operator* block);
   protected:
      virtual QLayout* leftLayout();
   };
}



#endif
