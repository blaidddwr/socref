#ifndef CPPQT_TYPELIST_VIEW_H
#define CPPQT_TYPELIST_VIEW_H
#include "cppqt_typelist.h"
#include "cppqt_base_view.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the type list block. 
    */
   class TypeList::View : public Base::View
   {
      Q_OBJECT
   public:
      explicit View(const TypeList* block);
   private:
      QString displayText();
      QString displayTypeLists();
      QString displayTypes();
      /*!
       * Pointer to the type list block this view is showing. 
       */
      const TypeList* _block;
   };
}



#endif
