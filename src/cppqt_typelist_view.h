#ifndef CPPQT_TYPELIST_VIEW_H
#define CPPQT_TYPELIST_VIEW_H
#include "cppqt_typelist.h"
#include "basicblock_view.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the type list block. 
    */
   class TypeList::View : public BasicBlock::View
   {
      Q_OBJECT
   public:
      explicit View(const TypeList* block);
   private slots:
      QString displayTypeLists();
      QString displayTypes();
   private:
      /*!
       * Pointer to the type list block this view is showing. 
       */
      const TypeList* _block;
   };
}



#endif
