#ifndef CPPQT_CLASSVIEW_H
#define CPPQT_CLASSVIEW_H
#include "basic_blockview.h"
#include "cppqt.h"
//



namespace CppQt
{
   /*!
    * This is the view class for the class block. 
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
