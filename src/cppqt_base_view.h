#ifndef CPPQT_BASE_VIEW_H
#define CPPQT_BASE_VIEW_H
#include "cppqt_base.h"
#include <QLabel>
//



namespace CppQt
{
   /*!
    * This is the base view class for its parent block. Because this is the base class 
    * that is not actually used as a concrete class this view class is also made in 
    * that way. This contained helper display functions that multiple concrete views 
    * use such as parsing bold text and making template argument lists. It also 
    * provides displaying its block's description field if a base block pointer is 
    * provided in its constructor. 
    */
   class Base::View : public QLabel
   {
   public:
      /*!
       * Constructs a new base view object with no associated base block. 
       */
      explicit View() = default;
      View(const Base* block);
   protected:
      static QString parseBoldMarkers(const QString& text);
      QString displayDescription();
      QString displayTemplates();
   private:
      /*!
       * Pointer to the base block this view is showing or null if the block being viewed 
       * does not inherit from the base block. 
       */
      const Base* _block {nullptr};
   };
}



#endif
