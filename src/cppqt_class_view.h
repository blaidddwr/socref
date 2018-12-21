#ifndef CPPQT_CLASS_VIEW_H
#define CPPQT_CLASS_VIEW_H
#include "cppqt_class.h"
#include "basicblock_view.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the class block. 
    */
   class Class::View : public BasicBlock::View
   {
      Q_OBJECT
   public:
      explicit View(const Class* block);
   private slots:
      QString displayQtObject();
      QString displayParents();
   private:
      /*!
       * The class block that this view is displaying. 
       */
      const Class* _block;
   };
}



#endif
