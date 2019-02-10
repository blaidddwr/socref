#ifndef CPPQT_CLASSVIEW_H
#define CPPQT_CLASSVIEW_H
#include "basic_blockview.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the class block. This provides custom displays that 
    * show if this view's class is a qt object, any parents it has, and any templates. 
    */
   class ClassView : public Basic::BlockView
   {
      Q_OBJECT
   public:
      explicit ClassView(const Class* block);
   private slots:
      QString displayQtObject();
      QString displayParents();
      QString displayTemplates();
   };
}



#endif
