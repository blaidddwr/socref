#ifndef CPPQT_CLASS_VIEW_H
#define CPPQT_CLASS_VIEW_H
#include "cppqt_class.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    */
   class Class::View : public Base::View
   {
      Q_OBJECT
   public:
      View(const Class* block);
   private:
      QString displayText();
      QString displayQtObject();
      QString displayParents();
      /*!
       * The variable block that this view is displaying. 
       */
      const Class* _block;
   };
}



#endif
