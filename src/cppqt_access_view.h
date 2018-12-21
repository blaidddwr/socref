#ifndef CPPQT_ACCESS_VIEW_H
#define CPPQT_ACCESS_VIEW_H
#include "cppqt_access.h"
#include "basicblock_view.h"
//



namespace CppQt
{
   /*!
    * This is the view widget for the access block type. 
    */
   class Access::View : public BasicBlock::View
   {
      Q_OBJECT
   public:
      explicit View(const Access* block);
   private slots:
      QString displayInfo();
   private:
      /*!
       * The access block that this view is displaying. 
       */
      const Access* _block;
   };
}



#endif
